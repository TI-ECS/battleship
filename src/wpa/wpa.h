#ifndef _WPA_H_
#define _WPA_H_

#include "device.h"
#include "interface.h"
#include "group.h"
#include "wps.h"
#include "p2pdevice.h"

#include <QObject>

class Wpa : public QObject
{
Q_OBJECT

public:
    Wpa(QObject *parent = 0);
    virtual ~Wpa();

public slots:
    void connectPeer(const QVariantMap &properties);
    void disconnectP2P();
    bool isEnabled();
    void setEnabled(bool enable);
    void setDeviceName(const QString &deviceName);
    void setProperties(const QVariantMap &properties);
    void startGroup();
    QString status();
    void stopGroup();
    void getPeers();
    QString getStatus();

private slots:
    void connectResult(QDBusPendingCallWatcher *watcher);
    void deviceWasFound(const QDBusObjectPath &path);
    void devicePropertiesChanged(const QVariantMap &properties);
    void disconnectResult(QDBusPendingCallWatcher *watcher);
    void findResult(QDBusPendingCallWatcher *watcher);
    void goNegotiationFailure(int status);
    void groupHasStarted(const QVariantMap &properties);
    void groupStartResult(QDBusPendingCallWatcher *watcher);
    void provisionDiscoveryPBCRequest(const QDBusObjectPath &peer_object);
    void wpsResult(QDBusPendingCallWatcher *watcher);

signals:
    void connectFails(int);
    void deviceFound(Device &device);
    void disconnected();
    void enabled(bool enable);
    void status(const QString &status);
    void groupStarted(bool go);
    void groupStartFails();

private:
    Q_PID wpaPid;;
    QString interfacePath;
    fi::w1::wpa_supplicant::InterfaceDevice *device;
    fi::w1::wpa_supplicant::Interface::WPS *wps;
    fi::w1::wpa_supplicant::Interface::P2PDevice *p2pInterface;

    void find();
    void setupDBus();
};

#endif /* _WPA_H_ */
