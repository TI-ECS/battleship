/*
  Copyright (c) 2000-2001 Nikolas Zimmermann <wildfox@kde.org>
            (c) 2000-2001 Daniel Molkentin <molkentin@kde.org>
            (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "playfield.h"

#include "controller.h"
#include "playerentity.h"
#include "seaview.h"
#include "simplemenu.h"
#include "stats.h"
#include "welcomescreen.h"

#include <QMessageBox>
#include <QStatusBar>
#include <QTimer>
#include <QVBoxLayout>

static const int MINIMUM_HEIGHT = 400;

PlayField::PlayField(QWidget* parent, QStatusBar* sbar)
: QWidget(parent)
, m_status_bar(sbar)
, m_show_endofgame_message(true)
{
    // setMinimumSize(static_cast<int>(MINIMUM_HEIGHT * 1.6), MINIMUM_HEIGHT);
    QVBoxLayout* layout = new QVBoxLayout;

    m_sea = new SeaView(this);
    layout->addWidget(m_sea, 1);
    connect(this, SIGNAL(rotate()), m_sea, SLOT(rotate()));

    layout->setMargin(0);
//     layout->setSpacing(0);
    setLayout(layout);

    m_controller = 0;
    m_menu = 0;
}

PlayField::~PlayField()
{
    // controller assumes that the view is still valid
    // when it is destroyed
    delete m_controller;
}

Controller* PlayField::createController()
{
    Controller* controller = new Controller(this);
    connect(controller, SIGNAL(gameOver(Sea::Player)),
            this, SLOT(gameOver(Sea::Player)));
    connect(controller, SIGNAL(gameAbort()),
            this, SLOT(gameAbort()));
    connect(controller, SIGNAL(restartRequested()),
            this, SLOT(restartRequested()));
    connect(controller, SIGNAL(compatibility(int)),
            this, SLOT(setCompatibility(int)));
    connect(controller, SIGNAL(turnChanged(int)),
            this, SLOT(changeTurn(int)));
    connect(controller, SIGNAL(playerReady(int)),
            this, SLOT(playerReady(int)));
    return controller;
}

void PlayField::setupController()
{
    Animator::instance()->restart();
    m_sea->clear();

    // remove welcome screen
    m_sea->screen(Sea::Player(0))->fadeOut();
    m_sea->screen(Sea::Player(1))->fadeOut();

    delete m_controller;
    m_controller = createController();
    m_menu->setupController(m_controller, 0, m_sea, false);
    startGame();
}

void PlayField::resetupController(bool ask)
{
    Entity* old_opponent = m_controller->findEntity(Sea::Player(1));
    if (old_opponent) {
        old_opponent->setParent(0);
    }
    delete m_controller;

    // create new controller
    m_controller = createController();
    m_menu->setupController(m_controller, old_opponent,
                            m_sea, ask);

    delete old_opponent;

    startGame();
}

void PlayField::endGame()
{
    Animator::instance()->restart();
    delete m_controller;

    m_controller = 0;
    m_sea->clear();
}

void PlayField::newGame()
{
    endGame();
    delete m_menu;

    m_sea->screen(Sea::Player(0))->show();
    m_sea->screen(Sea::Player(1))->show();

    m_menu = new SimpleMenu(this, m_sea->screen(Sea::Player(0)));
    connect(m_menu, SIGNAL(done()), this, SLOT(setupController()));
    emit welcomeScreen();
}

void PlayField::restart(bool ask)
{
    Animator::instance()->restart();
    m_sea->clear();
    resetupController(ask);
}

void PlayField::gameOver(Sea::Player winner)
{
    if (winner == Sea::Player(0)) {
        m_status_bar->showMessage(tr("You win!"));
        if (m_show_endofgame_message) {
            QMessageBox::information(this, tr("Result"), tr("You win. Excellent!"));
        }
    }
    else {
        m_status_bar->showMessage(tr("You lose."));
        if (m_show_endofgame_message) {
            QMessageBox::information(this, tr("Result"), tr("You lose. Try harder next time!"));
        }
    }

    // When we have finished, show again the welcome screen
    emit gameFinished();
}

void PlayField::restartRequested()
{
    int ans = QMessageBox::question(this, tr("Restart game"),
                    tr("Your opponent has requested to restart the game."));
    if (ans == QMessageBox::Ok) {
        restart(false);
    }
}

void PlayField::updateNick(int player, const QString& nick)
{
    m_sea->setNick(Sea::Player(player), nick);
}

void PlayField::changeTurn(int player)
{
    if (player == 0) {
        // local user
        m_status_bar->showMessage(tr("Enemy has shot. Shoot now!"));
    }
    else {
        // opponent
        m_status_bar->showMessage(tr("Waiting for enemy to shoot..."));
    }
}

void PlayField::playerReady(int player)
{
    if (player == -1) {
        // game can start
        if (m_controller->turn() == 0) {
            m_status_bar->showMessage(tr("Ships placed. Now shoot on the enemy field!"));
        }
        else {
            m_status_bar->showMessage(tr("Waiting for other player to start the game..."));
        }
    }
    else if (player == 0) {
        m_status_bar->showMessage(tr("Waiting for other player to place his ships..."));
    }
}

void PlayField::startGame()
{
    m_status_bar->showMessage(tr("Place your ships. Use the right mouse button to rotate them."));
    emit startingGame();
}

SimpleMenu* PlayField::createAuxMenu()
{
    SimpleMenu* menu = new SimpleMenu(this, 0);
    connect(menu, SIGNAL(done()), this, SLOT(auxMenuDone()));
    return menu;
}

void PlayField::auxMenuDone()
{
    qDebug() << "aux menu done";
    SimpleMenu* menu = qobject_cast<SimpleMenu*>(sender());
    if (menu) {
        delete m_menu;
        m_menu = menu;
        setupController();
    }
}

void PlayField::createServer()
{
    createAuxMenu()->createServer();
}

void PlayField::createClient()
{
    createAuxMenu()->createClient();
}

// void PlayField::createClient(const KUrl& url)
// {
//     createAuxMenu()->createClient(url);
// }

void PlayField::toggleEndOfGameMessage(bool show)
{
    m_show_endofgame_message = show;
}

void PlayField::toggleLeftGrid(bool show)
{
    m_sea->toggleLeftGrid(show);
}

void PlayField::toggleRightGrid(bool show)
{
    m_sea->toggleRightGrid(show);
}

void PlayField::gameAbort()
{
    m_status_bar->showMessage(tr("Game aborted!"));
    QMessageBox::information(this, tr("Result"),
                             tr("Your opponent disconnected from the game"));

    QTimer::singleShot(0, this, SLOT(newGame()));
}
