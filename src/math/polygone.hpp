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

#ifndef POLYGONE_HPP
#define POLYGONE_HPP

#include <vector>
#include <QPolygonF>
#include "coord.hpp"
#include "coord_qio.tpl"
#include "segment.hpp"

// Classe contenant un polygone.
class Polygone
{
public:
    // Constructeurs.
    inline Polygone();
    inline Polygone(const QList<Coord<double> >& sommets);

    // Lecture/écriture pour les fichiers de configuration.
    friend inline QDataStream& operator<<(QDataStream& stream, const Polygone& polygone);
    friend inline QDataStream& operator>>(QDataStream& stream, Polygone& polygone);

    // Efface les sommets du polygone.
    inline void clear();

    // Bornes du polygone.
    double left() const;
    double top() const;
    double right() const;
    double bottom() const;

    // Récupération des sommets et segments.
    inline unsigned int size() const;
    inline Segment segment(unsigned int i) const;
    inline Coord<double> point(unsigned int i) const;
    inline QList<Coord<double> > sommets() const;
    QPolygonF toPolygon() const;

    // Vérifie si un point est dans le polygone.
    bool inside(const Coord<double>& point) const;
    // Vérifie si un cercle croise le polygone.
    bool intersect(const Coord<double>& center, double rayon) const;

    // Calcule la surface du polygone.
    double surface() const;

private:
    // Liste de sommets.
    QList<Coord<double> > mSommets;
};

// Constructeurs.
inline Polygone::Polygone() :
    mSommets() {}
inline Polygone::Polygone(const QList<Coord<double> >& sommets) :
    mSommets(sommets) {}

// Lecture/écriture pour les fichiers de configuration.
inline QDataStream& operator<<(QDataStream& stream, const Polygone& polygone)
    {return stream << polygone.mSommets;}
inline QDataStream& operator>>(QDataStream& stream, Polygone& polygone)
    {return stream >> polygone.mSommets;}

// Efface les sommets du polygone.
inline void Polygone::clear()
    {mSommets.clear();}


// Récupération des sommets et segments.
inline unsigned int Polygone::size() const
    {return mSommets.size();}
inline Segment Polygone::segment(unsigned int i) const
    {return Segment(mSommets[i], mSommets[(i + 1) % mSommets.size()]);}
inline Coord<double> Polygone::point(unsigned int i) const
    {return mSommets[i];}

QList<Coord<double> > Polygone::sommets() const
    {return mSommets;}

#endif // POLYGONE_HPP
