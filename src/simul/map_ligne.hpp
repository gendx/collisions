/*
    Collisions - a real-time simulation program of colliding particles.
    Copyright (C) 2011 - 2014  G. Endignoux

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

#ifndef MAP_LIGNE_HPP
#define MAP_LIGNE_HPP

#include <QMap>
#include <set>
#include "boule.hpp"

// Classe contenant la liste des objets présents sur une bande horizontale de l'espace.
class MapLigne
{
public:
    // Constructeur.
    inline MapLigne();

    // Accesseurs.
    inline QMultiMap<int, Boule*>& boules();
    inline const QMultiMap<int, Boule*>& boules() const;
    inline QList<Piston*>& pistons();
    inline const QList<Piston*>& pistons() const;
    inline QMultiMap<int, Coord<double> >& sommets();
    inline const QMultiMap<int, Coord<double> >& sommets() const;
    inline QMap<int, std::set<Segment> >& segments();
    inline const QMap<int, std::set<Segment> >& segments() const;

private:
    // Boules.
    QMultiMap<int, Boule*> mBoules;
    // Pistons.
    QList<Piston*> mPistons;
    // Sommets des obstacles.
    QMultiMap<int, Coord<double> > mSommets;
    // Segments des obstacles.
    QMap<int, std::set<Segment> > mSegments;
};

// Constructeur.
inline MapLigne::MapLigne()
    {}

// Accesseurs.
inline QMultiMap<int, Boule*>& MapLigne::boules()
    {return mBoules;}
inline const QMultiMap<int, Boule*>& MapLigne::boules() const
    {return mBoules;}
inline QList<Piston*>& MapLigne::pistons()
    {return mPistons;}
inline const QList<Piston*>& MapLigne::pistons() const
    {return mPistons;}
inline QMultiMap<int, Coord<double> >& MapLigne::sommets()
    {return mSommets;}
inline const QMultiMap<int, Coord<double> >& MapLigne::sommets() const
    {return mSommets;}
inline QMap<int, std::set<Segment> >& MapLigne::segments()
    {return mSegments;}
inline const QMap<int, std::set<Segment> >& MapLigne::segments() const
    {return mSegments;}

#endif // MAP_LIGNE_HPP
