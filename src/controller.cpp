/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "controller.h"

#include "networkentity.h"
#include "playerentity.h"
#include "seaview.h"
#include "shot.h"


Controller::Controller(QObject* parent)
: QObject(parent)
, m_shot(0)
, m_ready(0)
{
    m_ui = 0;
    m_sea = new Sea(this, Coord(10, 10));
}

PlayerEntity* Controller::createPlayer(Sea::Player player, SeaView* view,
                                       const QString& nick)
{
    Q_UNUSED(nick);

    if (m_ui) {
        qDebug() << "Cannot create more than one human player";
        return 0;
    }
    PlayerEntity* entity = new PlayerEntity(player, m_sea, view);
    m_ui = entity;
    setupEntity(m_ui);
    return entity;
}

NetworkEntity* Controller::createRemotePlayer(Sea::Player player, Protocol* protocol, bool client)
{
    NetworkEntity* e = new NetworkEntity(player, m_sea, protocol, client);
    setupEntity(e);
    connect(e, SIGNAL(restartRequested()), this, SIGNAL(restartRequested()));
    if (client) {
        m_sea->switchTurn();
    }

    return e;
}

void Controller::setupEntity(Entity* entity)
{
    entity->setParent(this);

    connect(entity, SIGNAL(shoot(int,Coord)),
            this, SLOT(shoot(int,Coord)), Qt::QueuedConnection);
    connect(entity, SIGNAL(ready(int)),
            this, SLOT(ready(int)), Qt::QueuedConnection);
    connect(entity, SIGNAL(abortGame()),
            this, SIGNAL(gameAbort()));

    m_entities.append(entity);
}

bool Controller::allPlayers() const
{
    unsigned char bitmap = 0;
    foreach (Entity* entity, m_entities) {
        int player = entity->player();
        qDebug() << "found player" << player;
        bitmap |= (1 << player);
    }

    qDebug() << "bitmap =" << (unsigned) bitmap;
    return bitmap == 3;
}

bool Controller::start(SeaView* view, bool ask)
{
    if (!allPlayers()) {
        return false;
    }

    if (!m_ui) {
        m_ui = new UIEntity(Sea::NO_PLAYER, m_sea, view);
        setupEntity(m_ui);
    }

    foreach (Entity* entity, m_entities) {
        entity->start(ask);
    }

    return true;
}

void Controller::shoot(int player, const Coord& c)
{
    Entity* entity = findEntity(Sea::opponent(Sea::Player(player)));
    if (!entity) {
        qDebug() << "no entity!";
        return;
    }

    if (m_shot) {
        qDebug() << "shot in progress";
        // shot in progress
        return;
    }

    if (m_sea->status() == Sea::PLAYING) {
        entity->hit(m_shot = new Shot(this, Sea::Player(player), c)); // kind of CPS
    }
}

void Controller::finalizeShot(Sea::Player player, const Coord& c, const HitInfo& info)
{
    if (info.type != HitInfo::INVALID) {
        // notify entities
        notify(player, c, info);

        if (m_sea->status() == Sea::A_WINS) {
            finalizeGame(Sea::PLAYER_A);
        }
        else if (m_sea->status() == Sea::B_WINS) {
            finalizeGame(Sea::PLAYER_B);
        }
        else {
            emit turnChanged(m_sea->turn());
        }
    }
    else {
        qDebug() << "illegal move" << c << "for player" << player;
    }

    delete m_shot;
    m_shot = 0;
}

void Controller::notify(Sea::Player player, const Coord& c, const HitInfo& info)
{
    foreach (Entity* entity, m_entities) {
        entity->notify(player, c, info);
        if (player == entity->player()) {
            entity->stats()->addInfo(info);
        }
    }
}

void Controller::ready(int player)
{
    m_ready++;
    foreach (Entity* entity, m_entities) {
        entity->notifyReady(Sea::Player(player));
    }

    // when two entities are ready, start
    // all engines
    if (m_ready >= 2) {
        m_sea->startPlaying();
        foreach (Entity* entity, m_entities) {
            entity->startPlaying();
        }
        emit playerReady(-1);
    }
    else {
        emit playerReady(player);
    }
}

void Controller::finalizeGame(Sea::Player winner)
{
    foreach (Entity* entity, m_entities) {
        entity->notifyGameOver(winner);
    }
    emit gameOver(winner);
}

Entity* Controller::findEntity(Sea::Player player) const
{
    foreach (Entity* entity, m_entities) {
        if (entity->player() == player) {
            return entity;
        }
    }

    return 0;
}

Sea::Player Controller::turn() const
{
    return m_sea->turn();
}
