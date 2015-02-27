/*
    Collisions - a real-time simulation program of colliding particles.
    Copyright (C) 2011 - 2015  G. Endignoux

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.txt
*/

#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <QColor>
#include "polygone.hpp"

// Classe définissant un obstacle polygonal.
class Obstacle
{
public:
    // Constructeurs.
    inline Obstacle();
    inline Obstacle(const Polygone& sommets, const QColor& color);

    // Lecture/écriture pour les fichiers de configuration.
    friend inline QDataStream& operator<<(QDataStream& stream, const Obstacle& obstacle);
    friend inline QDataStream& operator>>(QDataStream& stream, Obstacle& obstacle);

    // Efface l'obstacle.
    inline void reset();

    // Accesseurs.
    inline const Polygone& sommets() const;
    inline QColor color() const;

private:
    // Données.
    Polygone mSommets;
    QColor mColor;
};

// Constructeurs.
inline Obstacle::Obstacle() :
    mSommets(), mColor(Qt::white) {}
inline Obstacle::Obstacle(const Polygone& sommets, const QColor& color) :
    mSommets(sommets), mColor(color) {}

// Lecture/écriture pour les fichiers de configuration.
inline QDataStream& operator<<(QDataStream& stream, const Obstacle& obstacle)
    {return stream << obstacle.mSommets << obstacle.mColor;}
inline QDataStream& operator>>(QDataStream& stream, Obstacle& obstacle)
    {return stream >> obstacle.mSommets >> obstacle.mColor;}

// Efface l'obstacle.
inline void Obstacle::reset()
    {mSommets.clear(); mColor = Qt::white;}

// Accesseurs.
inline const Polygone& Obstacle::sommets() const
    {return mSommets;}
inline QColor Obstacle::color() const
    {return mColor;}

#endif // OBSTACLE_HPP
