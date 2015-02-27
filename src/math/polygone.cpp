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

#include "polygone.hpp"

// Vérifie si un point est dans le polygone.
bool Polygone::inside(const Coord<double>& point) const
{
    bool result = mSommets.empty();
    for (int i = 0 ; i < mSommets.size() ; ++i)
    {
        Coord<double> segment(mSommets[(i + 1) % mSommets.size()] - mSommets[i]);
        double y = mSommets[i].y;
        double _y = mSommets[(i + 1) % mSommets.size()].y;

        if (((y <= point.y && _y > point.y) || (y > point.y && _y <= point.y))
            && (mSommets[i].x + (point.y - y) * segment.invCoeffDir() < point.x))
            result = !result;
    }
    return result;
}

// Vérifie si un cercle croise le polygone.
bool Polygone::intersect(const Coord<double>& center, double rayon) const
{
    for (int i = 0 ; i < mSommets.size() ; ++i)
    {
        Coord<double> segment(mSommets[(i + 1) % mSommets.size()] - mSommets[i]);
        Coord<double> vect(center - mSommets[i]);

        if ((std::abs(segment.det(vect)) <= rayon * segment.length() && segment.scalar(vect) >= 0 && segment.scalar(vect) <= segment.squareLength())
            || vect.length() <= rayon)
            return true;
    }
    return false;
}

// Convertit en polygone de Qt.
QPolygonF Polygone::toPolygon() const
{
    QPolygonF result;

    for (auto& sommet : mSommets)
        result.push_back(QPointF(sommet.x, sommet.y));
    return result;
}


// Bornes du polygone.
double Polygone::left() const
{
    double result = mSommets.front().x;

    for (int i = 1 ; i < mSommets.size() ; ++i)
        if (mSommets[i].x < result)
            result = mSommets[i].x;

    return result;
}

double Polygone::top() const
{
    double result = mSommets.front().y;

    for (int i = 1 ; i < mSommets.size() ; ++i)
        if (mSommets[i].y < result)
            result = mSommets[i].y;

    return result;
}

double Polygone::right() const
{
    double result = mSommets.front().x;

    for (int i = 1 ; i < mSommets.size() ; ++i)
        if (mSommets[i].x > result)
            result = mSommets[i].x;

    return result;
}

double Polygone::bottom() const
{
    double result = mSommets.front().y;

    for (int i = 1 ; i < mSommets.size() ; ++i)
        if (mSommets[i].y > result)
            result = mSommets[i].y;

    return result;
}


// Calcule la surface du polygone.
double Polygone::surface() const
{
    double result = (mSommets.back().x + mSommets.front().x) * (mSommets.front().y - mSommets.back().y) / 2.0;

    for (int i = 1 ; i < mSommets.size() ; ++i)
        result += (mSommets[i-1].x + mSommets[i].x) * (mSommets[i].y - mSommets[i-1].y) / 2.0;

    return std::fabs(result);
}
