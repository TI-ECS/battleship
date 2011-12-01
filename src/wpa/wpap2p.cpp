#include "wpap2p.h"

#include <QDebug>
#include <QDir>
#include <QTimer>
#include <signal.h>
#include <sys/types.h>

#define CREATE_CONNECTION "p2p_connect %1 pbc join\n"
#define CREATE_GROUP "p2p_group_add\n"
#define GET_PEER "p2p_peer %1\n"
#define GET_PEERS "p2p_peers\n"
#define GET_STATUS "status\n"
#define P2P_FIND "p2p_find\n"
#define REMOVE_GROUP "p2p_group_remove %1\n"
#define SET_CHANNEL "p2p_set listen_channel %1\n"
#define SET_COMMAND "set %1 %2\n"
#define WPA_PROCESS_NAME "wpa_supplicant"
#define TIMEOUT 8000           // 8s


Q_PID proc_find(const QString &name)
{
    bool ok;
    QDir dir;

    dir = QDir("/proc");
    if (!dir.exists()) {
        qCritical() << "can't open /proc";
        return -1;
    }

    foreach(QString fileName, dir.entryList()) {
        long lpid = fileName.toLong(&ok, 10);
        if (!ok)
            continue;

        QFile file(QString("/proc/%1/cmdline").arg(lpid));
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QByteArray cmd = file.readLine();
            if (cmd.contains(name.toAscii())) {
                file.close();
                return (Q_PID)lpid;
            }
            file.close();
        }
    }

    return -1;
}

WPAp2p::WPAp2p(QObject *parent)
    :QThread(parent),
     WPAProcess(0)
{
    active = hasGroup = false;
    currentAction = NONE;
    connect(&WPAProcess, SIGNAL(readyReadStandardOutput()),
            this, SLOT(readWPAStandartOutput()));

    WPAPid = proc_find(WPA_PROCESS_NAME);

#if defined(DEBUG)
    logFile.setFileName("/home/root/wifi-direct.log");
    logFile.open(QIODevice::WriteOnly | QIODevice::Text);
#endif
}

WPAp2p::~WPAp2p()
{
#if defined(DEBUG)
    logFile.close();
#endif

    WPAProcess.close();
    WPAProcess.kill();
}

void WPAp2p::connectPBC(const QString &device)
{
    QString connection;
    connection = QString(CREATE_CONNECTION).arg(device);

    mutex.lock();
    ActionValue action = {CONNECT, connection};
    actionsQueue.enqueue(action);
    mutex.unlock();
}

int WPAp2p::exec()
{
    while (active) {
        sleep(1.2);
        mutex.lock();
        if (currentAction == NONE) {
            if (!actionsQueue.isEmpty()) {
		WPAProcess.waitForBytesWritten(-1);
                ActionValue action = actionsQueue.dequeue();
                currentAction = action.action;

                switch (action.action) {
                case CHANGE_CHANNEL:
                    WPAProcess.write(QString(SET_CHANNEL).
                                     arg(action.value.toInt()).toAscii());
                    break;
                case CHANGE_INTENT:
                    WPAProcess.write(QString(SET_COMMAND).arg("p2p_go_intent").
                                     arg(action.value.toInt()).toAscii());
                    break;
                case CONNECT:
                    WPAProcess.write(action.value.toString().toAscii());
                    break;
                case GETTING_PEER_INFORMATION:
                {
                    QString d(action.value.toString());
                    currentDevice = d;
                    WPAProcess.write(QString(GET_PEER).
                                     arg(d).toAscii());
                }
                    break;
                case GETTING_STATUS:
                    WPAProcess.write(GET_STATUS);
                    break;
                case SCANNING:
                    WPAProcess.write(QString(P2P_FIND).toAscii());
                    break;
                case SCAN_RESULT:
                    WPAProcess.write(GET_PEERS);
                    break;
                case SETTING_NAME:
                    WPAProcess.write(QString(SET_COMMAND).arg("device_name").
                                     arg(action.value.toString()).toAscii());
                    break;
                case START_GROUP:
                    WPAProcess.write(CREATE_GROUP);
                    break;
                case STOP_GROUP:
                    WPAProcess.write(QString(REMOVE_GROUP).
                                     arg("wlan0").toAscii());
                    break;
                default:
                    break;
                }
            }
        }
        mutex.unlock();
    }

    return 0;
}

void WPAp2p::getPeer()
{
    if (WPAPid == -1) return;

    mutex.lock();
    ActionValue action = {SCAN_RESULT, 0};
    actionsQueue.enqueue(action);
    mutex.unlock();
}

void WPAp2p::getPeers()
{
    if (WPAPid == -1) return;

    mutex.lock();
    ActionValue action = {SCAN_RESULT, 0};
    actionsQueue.enqueue(action);
    mutex.unlock();
}

void WPAp2p::readWPAStandartOutput()
{
    QString value(WPAProcess.read(WPAProcess.bytesAvailable()));
#if defined(DEBUG)
    logFile.write(QString("Current action: %1").
                  arg(currentAction).toAscii());
    logFile.write(value.toAscii());
#endif
    if (currentAction == NONE)
        return;

    ActionValue actionStatus = {GETTING_STATUS, 0};
    int index;

    mutex.lock();
    switch (currentAction) {
    case GETTING_STATUS:
        if ((index = value.indexOf("wpa_state=")) > -1)
            emit status(value.mid(index + 10, value.indexOf("\n", index)
                                  - index - 10));
        else
            return;
        break;
    case GETTING_PEER_INFORMATION:
    {
        static QString buf;
        buf.append(value);
        if (buf.endsWith("> ")) {
            devices[currentDevice].data()->setValues(buf);
            emit deviceUpdate(devices[currentDevice].data());
            emit deviceFound(devices[currentDevice]);
            buf.clear();
        } else {
            goto end;
        }
    }
        break;
    case START_GROUP:
        if (value.contains("OK")) {
            emit groupStarted();
            hasGroup = true;
            actionsQueue.enqueue(actionStatus);
        }
        break;
    case STOP_GROUP:
        if (value.contains("OK")) {
            emit groupStopped();
            hasGroup = false;
            actionsQueue.enqueue(actionStatus);
        }
        break;
    case SCANNING:
        if (value.contains("FAIL"))
            qDebug() << "Scanning fails";
        break;
    case SETTING_NAME:
        if (value.contains("FAIL"))
            qDebug() << "Setting name fails";
        break;
    case SCAN_RESULT:
    {
        QString buffer;
        buffer.append(value);
        if (buffer.endsWith("> ")) {
            QStringList devs = buffer.split("\n");
            foreach (const QString &dev, devs) {
                if (dev.startsWith(">"))
                    continue;
                devices[dev] = QSharedPointer<Device>(new Device(dev));
                ActionValue action = {GETTING_PEER_INFORMATION, dev};
                actionsQueue.enqueue(action);
            }
            buffer.clear();
        } else {
            goto end;
        }
    }
        break;
    case CONNECT:
    {
        QString buffer;
        buffer.append(value);
        if (buffer.endsWith("> ")) {
            emit connectCommandFinished();
            buffer.clear();
        } else
            goto end;
    }
        break;
    case CHANGE_INTENT:
        if (value.contains("FAIL"))
            qDebug() << "Change intent fails";
    case CHANGE_CHANNEL:
        if (value.contains("FAIL"))
            qDebug() << "Setting channel fails";
    default: break;
    }

    currentAction = NONE;

end:
    mutex.unlock();
}

void WPAp2p::run()
{
    active = true;
    exec();

    QThread::exit();
}

void WPAp2p::scan()
{
    if (WPAPid == -1) return;

    mutex.lock();
    ActionValue action = {SCANNING, 0};
    actionsQueue.enqueue(action);
    QTimer::singleShot(TIMEOUT, this, SLOT(getPeers()));
    mutex.unlock();
}

void WPAp2p::setChannel(int value)
{
    if (WPAPid == -1) return;

    mutex.lock();
    ActionValue action = {CHANGE_CHANNEL, value};
    actionsQueue.enqueue(action);
    mutex.unlock();
}

void WPAp2p::setIntent(int value)
{
    if (WPAPid == -1) return;

    mutex.lock();
    ActionValue action = {CHANGE_INTENT, value};
    actionsQueue.enqueue(action);
    mutex.unlock();
}

void WPAp2p::start(Priority priority)
{
    if (WPAPid != -1) {

        WPAProcess.start("/usr/sbin/wpa_cli");
        if (!WPAProcess.waitForStarted(-1))
            return;
        scan();
    } else {
        currentAction = NONE;
    }

    QThread::start(priority);
}

void WPAp2p::startGroup()
{
    if (WPAPid == -1) return;

    ActionValue action;
    if (hasGroup)
        action.action = STOP_GROUP;
    else
        action.action = START_GROUP;

    mutex.lock();
    actionsQueue.enqueue(action);
    mutex.unlock();
}

void WPAp2p::setEnabled(bool state)
{
    if (state) {
        if (WPAPid != -1) return;

        if (QProcess::startDetached(WPA_PROCESS_NAME,
                                    QStringList() << "d" << "-Dnl80211"
                                    << "-c/etc/wpa_supplicant.conf" << "-iwlan0"
                                    << "-B", QDir::rootPath(), &WPAPid)) {
            WPAPid += 1;        // It's really weird, but startDetached is
                                // it's always returning the pid - 1.
            this->sleep(6);     // waiting the wpa_cli reconnects.
            WPAProcess.start("/usr/sbin/wpa_cli");
            if (!WPAProcess.waitForStarted(-1))
                return;
        }
    } else {
        WPAProcess.close();
        if (WPAPid == -1)
            return;
        if (kill(WPAPid, SIGKILL) != -1) {
            WPAPid = -1;
            emit enabled(false);
            actionsQueue.clear();
            currentAction = NONE;
        }
    }
}

void WPAp2p::setName(const QString &value)
{
    if (WPAPid == -1) return;

    mutex.lock();

    ActionValue action = {SETTING_NAME, value};
    actionsQueue.enqueue(action);

    mutex.unlock();
}

void WPAp2p::stop()
{
    mutex.lock();
    active = false;
    mutex.unlock();
}

bool WPAp2p::startGroupServer(const QString &name)
{
    qint64 wpaPid = proc_find(WPA_PROCESS_NAME);
    QProcess process;

    if (wpaPid == -1) {
        if (QProcess::startDetached(WPA_PROCESS_NAME,
                                    QStringList() << "d" << "-Dnl80211"
                                    << "-c/etc/wpa_supplicant.conf" << "-iwlan0"
                                    << "-B", QDir::rootPath(), &wpaPid)) {
            wpaPid += 1;        // It's really weird, but startDetached is
                                // it's always returning the pid - 1.
            sleep(7);     // waiting the wpa_cli reconnects.
            process.start("/usr/sbin/wpa_cli");
            if (!process.waitForStarted(-1))
                return false;
        } else {
            return false;
        }
    } else {
        process.start("/usr/sbin/wpa_cli");
        if (!process.waitForStarted(-1))
            return false;
    }

    // // making visible
    process.write(QString(P2P_FIND).toAscii());
    process.waitForBytesWritten(-1);

    // setting properties
    process.write(QString(SET_COMMAND).arg("device_name").
                  arg(name).toAscii());
    process.waitForBytesWritten(-1);
    process.write(QString(SET_COMMAND).arg("manufacturer").
                  arg("battleship").toAscii());
    process.waitForBytesWritten(-1);
    process.write(QString(SET_COMMAND).arg("model_number").
                  arg("1234").toAscii());
    process.waitForBytesWritten(-1);

    // starting group
    process.write(QString(CREATE_GROUP).toAscii());
    process.waitForBytesWritten(-1);
    sleep(4);

    process.write(QString("wps_pbc\n").toAscii());
    process.waitForBytesWritten(-1);

    sleep(1);

    return true;
}

bool WPAp2p::connectToGroup(const QString &address)
{
    qint64 wpaPid = proc_find(WPA_PROCESS_NAME);
    QProcess process;

    if (wpaPid == -1) {
        qDebug() << "teste";
        if (QProcess::startDetached(WPA_PROCESS_NAME,
                                    QStringList() << "d" << "-Dnl80211"
                                    << "-c/etc/wpa_supplicant.conf" << "-iwlan0"
                                    << "-B", QDir::rootPath(), &wpaPid)) {
            wpaPid += 1;        // It's really weird, but startDetached is
                                // it's always returning the pid - 1.
            sleep(5);     // waiting the wpa_cli reconnects.
            process.start("/usr/sbin/wpa_cli");
            if (!process.waitForStarted(-1))
                return false;
        } else {
            return false;
        }
    } else {
        process.start("/usr/sbin/wpa_cli");
        if (!process.waitForStarted(-1))
            return false;
    }

    // // making visible
    sleep(2);
    process.waitForBytesWritten(-1);
    process.write(QString(CREATE_CONNECTION).arg(address).toAscii());
    qDebug() <<QString(CREATE_CONNECTION).arg(address).toAscii();
    process.waitForBytesWritten(-1);

    sleep(6);
    return true;
}

bool WPAp2p::setInterfaceIp(const QString &ip)
{
    QStringList args;
    args << "wlan0" << ip;
    int i = QProcess::execute("/sbin/ifconfig", args);
    if (i < 0)
        return false;

    return !i;
}
