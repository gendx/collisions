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

#include "segment.hpp"

#include "coord_io.tpl"

// Affichage dans un flux standard.
std::ostream& operator<<(std::ostream& flux, const Segment& segment)
{
    flux << "( " << segment.mPoint1 << " -> " << segment.mVecteur << " )";
    return flux;
}

// Vérifie si la projection orthogonale d'un point est sur le segment.
bool Segment::face(const Coord<double>& point) const
{
    Coord<double> vect(point - mPoint1);
    return mVecteur.scalar(vect) >= 0 && mVecteur.scalar(vect) <= mVecteur.squareLength();
}
