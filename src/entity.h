/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef ENTITY_H
#define ENTITY_H

#include "sea.h"
#include "stats.h"

#include <QObject>

class Shot;
class QIcon;

class Entity : public QObject
{
Q_OBJECT
public:

protected:
    Sea::Player m_player;
    QString m_nick;
    Stats m_stats;
public:
    Entity(Sea::Player player);
    virtual ~Entity();
    virtual void notify(Sea::Player player, const Coord& c, const HitInfo& info) = 0;
    virtual void hit(Shot* shot) = 0;
    virtual void start(bool) = 0;
    virtual void startPlaying() { }
    virtual void notifyReady(Sea::Player) { }
    virtual void notifyGameOver(Sea::Player) { }
    Stats* stats();

    virtual Sea::Player player() const { return m_player; }
    virtual QIcon icon() const = 0;
signals:
    void shoot(int player, const Coord& c);
    void ready(int player);
    void abortGame();
};

#endif // ENTITY_H

