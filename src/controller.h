/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "sea.h"

class Entity;
class NetworkEntity;
class UIEntity;
class PlayerEntity;
class SeaView;
class Shot;
class Protocol;

class Controller : public QObject
{
Q_OBJECT
    QList<Entity*> m_entities;
    UIEntity* m_ui;
    Sea* m_sea;
    Shot* m_shot;
    int m_ready;

    void notify(Sea::Player player, const Coord& c, const HitInfo& info);
    void setupEntity(Entity*);
    void finalizeShot(Sea::Player player, const Coord& c, const HitInfo& info);
    void finalizeGame(Sea::Player winner);
    bool allPlayers() const;


    friend class Shot;
public:
    explicit Controller(QObject* parent);

    PlayerEntity* createPlayer(Sea::Player player, SeaView* view,
                               const QString& nick);
    NetworkEntity* createRemotePlayer(Sea::Player player, Protocol* protocol, bool client);

    bool start(SeaView* view, bool ask = false);
    Entity* findEntity(Sea::Player) const;
    Sea::Player turn() const;
public slots:
    void shoot(int player, const Coord& c);
    void ready(int player);
signals:
    void gameAbort();
    void gameOver(Sea::Player);
    void restartRequested();
    void turnChanged(int);
    void playerReady(int); // -1 means all players are ready
};

#endif // CONTROLLER_H

