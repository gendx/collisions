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

#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <iostream>
#include "coord.tpl"

// Classe contenant un segment.
class Segment
{
public:
    // Constructeurs.
    inline Segment();
    inline Segment(const Coord<double>& point1, const Coord<double>& point2);

    // Affichage dans un flux standard.
    friend std::ostream& operator<<(std::ostream& flux, const Segment& segment);

    // Comparaison lexicographique.
    inline bool operator<(const Segment& segment) const;
    inline bool operator==(const Segment& segment) const;
    // Vecteur associé : p2 - p1.
    inline const Coord<double>& vect() const;
    // Vecteur : point - p1.
    inline Coord<double> vect(const Coord<double>& point) const;

    // Longueur du segment.
    inline double length() const;
    inline double squareLength() const;

    // Vérifie si la projection orthogonale d'un point est sur le segment.
    bool face(const Coord<double>& point) const;

    // Case du quadrillage de pas "sizeArea" contenant le point.
    inline Coord<int> point1(double sizeArea) const;
    inline Coord<int> point2(double sizeArea) const;

    // Projection sur le segment parallèlement au repère.
    inline double yAtX(double x) const;
    inline double xAtY(double y) const;

private:
    Coord<double> mPoint1;
    Coord<double> mVecteur;
};

// Constructeurs.
inline Segment::Segment() {}
inline Segment::Segment(const Coord<double>& point1, const Coord<double>& point2) :
    mPoint1(point1), mVecteur(point2 - point1) {}

// Comparaison lexicographique.
inline bool Segment::operator<(const Segment& segment) const
    {return mPoint1 == segment.mPoint1 ? mVecteur < segment.mVecteur : mPoint1 < segment.mPoint1;}
inline bool Segment::operator==(const Segment& segment) const
    {return mPoint1 == segment.mPoint1 && mVecteur == segment.mVecteur;}
// Vecteur associé : p2 - p1.
inline const Coord<double>& Segment::vect() const
    {return mVecteur;}
// Vecteur : point - p1.
inline Coord<double> Segment::vect(const Coord<double>& point) const
    {return point - mPoint1;}

// Longueur du segment.
inline double Segment::length() const
    {return mVecteur.length();}
inline double Segment::squareLength() const
    {return mVecteur.squareLength();}

// Case du quadrillage de pas "sizeArea" contenant le point.
inline Coord<int> Segment::point1(double sizeArea) const
    {return Coord<int>(std::floor(mPoint1.x / sizeArea), std::floor(mPoint1.y / sizeArea));}
inline Coord<int> Segment::point2(double sizeArea) const
    {return Coord<int>(std::floor((mPoint1.x + mVecteur.x) / sizeArea), std::floor((mPoint1.y + mVecteur.y) / sizeArea));}

// Projection sur le segment parallèlement au repère.
inline double Segment::yAtX(double x) const
    {return mPoint1.y + (x - mPoint1.x) * mVecteur.coeffDir();}
inline double Segment::xAtY(double y) const
    {return mPoint1.x + (y - mPoint1.y) * mVecteur.invCoeffDir();}

#endif // SEGMENT_HPP
