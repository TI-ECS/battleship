/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "simplemenu.h"

#include <QTcpSocket>
#include <QTcpServer>
#include <QIcon>
#include <QMessageBox>

#include "button.h"
#include "controller.h"
//#include "networkdialog.h"
#include "playerentity.h"
#include "protocol.h"
#include "networkentity.h"
#include "seaview.h"
#include "welcomescreen.h"

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
        m_server_btn = m_screen->addButton(0, 0, QIcon(QLatin1String(iconServer)), tr("Host Network Game"));
        m_client_btn = m_screen->addButton(0, 1, QIcon(QLatin1String(iconClient)), tr("Connect to Network Game"));

        // create connections
        connect(m_server_btn, SIGNAL(clicked()),
            this, SLOT(createServer()));
        connect(m_client_btn, SIGNAL(clicked()),
            this, SLOT(createClient()));
    }
}

void SimpleMenu::finalize(State state, const QString& nickname, QTcpSocket* socket)
{
    m_state = state;
    m_nickname = nickname;
    if (socket) {
        m_protocol = new Protocol(socket);
        m_protocol->setParent(this);
    }
    else {
        m_protocol = 0;
    }
    emit done();
}

void SimpleMenu::createServer()
{
    QWidget* parent_widget = qobject_cast<QWidget*>(parent());
    Q_ASSERT(parent_widget);

    QTcpServer s(this);
    QMessageBox dialog(parent_widget);
    dialog.setText("Waiting for other player to connect...");
    dialog.addButton(QMessageBox::Cancel);
    connect(&s, SIGNAL(newConnection()), &dialog, SLOT(accept()));

    s.listen(QHostAddress::Any, 1234);
    if(dialog.exec() == QDialog::Accepted)
        finalize(DONE_SERVER, tr("Me"), s.nextPendingConnection());
}

void SimpleMenu::createClient()
{
    QWidget* parent_widget = qobject_cast<QWidget*>(parent());
    Q_ASSERT(parent_widget);
    //FIXME use avahi and don't block
    QTcpSocket *s = new QTcpSocket(this);
    s->connectToHost(QHostAddress::LocalHost, 1234);
    s->waitForConnected(-1);
    finalize(DONE_CLIENT, tr("Me"), s);
}

void SimpleMenu::setupController(Controller* controller, Entity* old_opponent, SeaView* sea,
                                 bool ask)
{
    switch (m_state) {
    case DONE_SERVER: {
        Q_ASSERT(m_protocol);
        m_player1 = controller->createPlayer(Sea::Player(0), sea, m_nickname);
        sea->setStats(Sea::Player(0), "score_mouse",
                      m_nickname, m_player1->stats());
        m_player2 = controller->createRemotePlayer(Sea::Player(1), m_protocol, false);
        if (old_opponent) {
            m_player2->setNick(old_opponent->nick());
        }
        sea->setStats(Sea::Player(1), "score_network",
                      m_player2->nick().isEmpty() ? tr("Remote player") : m_player2->nick(),
                      m_player2->stats());
        break;
    }
    case DONE_CLIENT: {
        Q_ASSERT(m_protocol);
        m_player1 = controller->createPlayer(Sea::Player(0), sea, m_nickname);
        sea->setStats(Sea::Player(0), "score_mouse",
                      m_nickname, m_player1->stats());
        m_player2 = controller->createRemotePlayer(Sea::Player(1), m_protocol, true);
        if (old_opponent) {
            m_player2->setNick(old_opponent->nick());
        }
        sea->setStats(Sea::Player(1), "score_network",
                      m_player2->nick().isEmpty() ? tr("Remote player") : m_player2->nick(),
                      m_player2->stats());
        break;
    }
    default:
        return;
    }

    controller->start(sea, ask);
}
