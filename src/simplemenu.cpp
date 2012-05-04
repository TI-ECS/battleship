/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "simplemenu.h"

#include "button.h"
#include "clientnetworkdialog.h"
#include "controller.h"
#include "networkentity.h"
#include "playerentity.h"
#include "protocol.h"
#include "seaview.h"
#include "welcomescreen.h"

#include "wpa/wpa.h"

#include <QHostInfo>
#include <QIcon>
#include <QMessageBox>
#include <QTcpServer>
#include <QTcpSocket>

const char* SimpleMenu::iconServer = ":/data/network-server.png";
const char* SimpleMenu::iconClient = ":/data/network-connect.png";

SimpleMenu::SimpleMenu(QWidget* parent, WelcomeScreen* screen)
: QObject(parent)
, m_screen(screen)
, m_protocol(0)
, m_state(READY)
, m_player1(0)
, m_player2(0)
{
    if (m_screen) {
        // create buttons
        m_server_btn = m_screen->addButton(0, 0, QIcon(QLatin1String(iconServer)),
                                           tr("Host Game"));
        m_client_btn = m_screen->addButton(0, 1, QIcon(QLatin1String(iconClient)),
                                           tr("Connect to Game"));

        // create connections
        connect(m_server_btn, SIGNAL(clicked()),
            this, SLOT(createServer()));
        connect(m_client_btn, SIGNAL(clicked()),
            this, SLOT(createClient()));

        // WiFi direct
        wpa = new Wpa;
        wpa->setEnabled(true);

        QVariantMap args;
        args["DeviceName"] = QHostInfo::localHostName();
        const char dev_type[8] = {0x00, 0x09, 0x00, 0x50, 0xf2, 0x04, 0x00, 0x05};
        args["PrimaryDeviceType"] = QByteArray(dev_type, 8);
        wpa->setProperties(args);
    }
}

void SimpleMenu::finalize(State state, const QString& nickname, QTcpSocket* socket)
{
    m_state = state;
    m_nickname = nickname;

    if (socket) {
        m_protocol = new Protocol(socket);
        m_protocol->setParent(this);
    } else {
        m_protocol = 0;
    }

    emit done();
}

void SimpleMenu::createServer()
{
    QWidget* parent_widget = qobject_cast<QWidget*>(parent());
    Q_ASSERT(parent_widget);

    QMessageBox dialog(parent_widget);
    QTcpServer s(this);

    dialog.setText("Waiting for other player to connect...");
    wpa->stopGroup();
    wpa->disconnectP2P();
    wpa->startGroup();

    dialog.addButton(QMessageBox::Cancel);

    connect(&s, SIGNAL(newConnection()), &dialog, SLOT(accept()));
    s.listen(QHostAddress::Any, 1234);

    if (dialog.exec() == QDialog::Accepted)
        finalize(DONE_SERVER, tr("Me"), s.nextPendingConnection());
    else if (wpa->status() == "completed")
            wpa->stopGroup();
}

void SimpleMenu::createClient()
{
    ClientNetworkDialog dialog;

    if(dialog.exec()) {
        wpa->stopGroup();
        wpa->disconnectP2P();
        connect(wpa, SIGNAL(groupStarted(bool)), this,
                SLOT(connectToHost(bool)));
    }
}

void SimpleMenu::setupController(Controller* controller, Entity* old_opponent,
                                 SeaView* sea, bool ask)
{
    Q_UNUSED(old_opponent);

    switch (m_state) {
    case DONE_SERVER: {
        Q_ASSERT(m_protocol);
        m_player1 = controller->createPlayer(Sea::Player(0), sea, m_nickname);
        sea->setStats(Sea::Player(0), "score_mouse",
                      m_nickname, m_player1->stats());
        m_player2 = controller->createRemotePlayer(Sea::Player(1),
                                                   m_protocol, false);
        sea->setStats(Sea::Player(1), "score_network", tr("Opponent"),
                      m_player2->stats());
        break;
    }
    case DONE_CLIENT: {
        Q_ASSERT(m_protocol);
        m_player1 = controller->createPlayer(Sea::Player(0), sea, m_nickname);
        sea->setStats(Sea::Player(0), "score_mouse",
                      m_nickname, m_player1->stats());
        m_player2 = controller->createRemotePlayer(Sea::Player(1),
                                                   m_protocol, true);
        sea->setStats(Sea::Player(1), "score_network", tr("Opponent"),
                      m_player2->stats());
        break;
    }
    default:
        return;
    }

    controller->start(sea, ask);
}

void SimpleMenu::connectToHost(bool go)
{
    sleep(4); // Hack for p2p, it lets the wifi-direct do its job

    if (!go) {
	QTcpSocket *s = new QTcpSocket(this);
	s->connectToHost("192.168.0.1", 1234);
	if (s->waitForConnected(-1))
            finalize(DONE_CLIENT, tr("Me"), s);
    }
}
