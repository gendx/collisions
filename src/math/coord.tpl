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

#ifndef COORD_TPL
#define COORD_TPL

#include "coord.hpp"

#include <cmath>
#include <algorithm>

// constructeur : coordonnées de départ, d'arrivée, et éventuellement le pas entre 2 coordonnées itérées
template <typename T>
Coord<T>::Iterator::Iterator(const Coord<T>& begin, const Coord<T>& end) :
    mCurrent(begin),
    mBegin(begin),
    mEnd(end),
    mActive(mBegin.x < mEnd.x && mBegin.y < mEnd.y),
    mNewLine(false)
{
}

// déréférence l'itérateur
template <typename T>
const Coord<T>& Coord<T>::Iterator::operator*() const
{
    return mCurrent;
}

template <typename T>
const Coord<T>* Coord<T>::Iterator::operator->() const
{
    return &mCurrent;
}

// vérifie si l'itérateur est actif
template <typename T>
bool Coord<T>::Iterator::active() const
{
    return mActive;
}

// vérifie si l'itérateur entre dans une nouvelle ligne
template <typename T>
bool Coord<T>::Iterator::new_line()
{
    if (mNewLine)
    {
        mNewLine = false;
        return true;
    }
    return false;
}

// vecteur nul
template <typename T>
Coord<T>::Coord() :
    x(0),
    y(0)
{
}

// vecteur
template <typename T>
Coord<T>::Coord(const T& X, const T& Y) :
    x(X),
    y(Y)
{
}

// y = x
template <typename T>
Coord<T>::Coord(const T& X) :
    x(X),
    y(X)
{
}


// transtypage
template <typename T>
template <typename _T>
Coord<T>::Coord(const Coord<_T>& coord) :
    x(T(coord.x)),
    y(T(coord.y))
{
}


// x == y ?
template <typename T>
bool Coord<T>::isSquare() const
{
    return x == y;
}

// compare les vecteurs
template <typename T>
bool Coord<T>::operator==(const Coord<T>& coord) const
{
    return x == coord.x && y == coord.y;
}

template <typename T>
bool Coord<T>::operator!=(const Coord<T>& coord) const
{
    return x != coord.x || y != coord.y;
}


// ordre lexicographique
template <typename T>
bool Coord<T>::operator<(const Coord<T>& coord) const
{
    return x == coord.x ? y < coord.y : x < coord.x;
}


// addition de vecteurs
template <typename T>
void Coord<T>::operator+=(const Coord<T>& coord)
{
    x += coord.x;
    y += coord.y;
}

template <typename T>
void Coord<T>::operator+=(const T& move)
{
    x += move;
    y += move;
}

template <typename T>
Coord<T> Coord<T>::operator+(const Coord<T>& coord) const
{
    Coord<T> tmp(*this);
    tmp += coord;
    return tmp;
}

template <typename T>
Coord<T> Coord<T>::operator+(const T& move) const
{
    Coord<T> tmp(*this);
    tmp += move;
    return tmp;
}

// soustraction de vecteurs
template <typename T>
void Coord<T>::operator-=(const Coord<T>& coord)
{
    x -= coord.x;
    y -= coord.y;
}

template <typename T>
Coord<T> Coord<T>::operator-(const Coord<T>& coord) const
{
    Coord<T> tmp(*this);
    tmp -= coord;
    return tmp;
}

template <typename T>
Coord<T> Coord<T>::operator-() const
{
    return Coord<T>(-x, -y);
}

// multiplie chaque composante du vecteur par la même composante de l'autre vecteur
template <typename T>
void Coord<T>::operator*=(const Coord<T>& coord)
{
    x *= coord.x;
    y *= coord.y;
}

template <typename T>
Coord<T> Coord<T>::operator*(const Coord<T>& coord) const
{
    Coord<T> tmp(*this);
    tmp *= coord;
    return tmp;
}

// multiplie chaque composante du vecteur par le ratio
template <typename T>
void Coord<T>::operator*=(const T& ratio)
{
    x *= ratio;
    y *= ratio;
}

template <typename T>
Coord<T> Coord<T>::operator*(const T& ratio) const
{
    Coord<T> tmp(*this);
    tmp *= ratio;
    return tmp;
}

// divise chaque composante du vecteur par la même composante de l'autre vecteur
template <typename T>
void Coord<T>::operator/=(const Coord<T>& coord)
{
    x /= coord.x;
    y /= coord.y;
}

template <typename T>
Coord<T> Coord<T>::operator/(const Coord<T>& coord) const
{
    Coord<T> tmp(*this);
    tmp /= coord;
    return tmp;
}

// divise chaque composante du vecteur par le ratio
template <typename T>
void Coord<T>::operator/=(const T& ratio)
{
    x /= ratio;
    y /= ratio;
}

template <typename T>
Coord<T> Coord<T>::operator/(const T& ratio) const
{
    Coord<T> tmp(*this);
    tmp /= ratio;
    return tmp;
}


// _x = min(x, x')
// _y = min(y, y')
template <typename T>
Coord<T> Coord<T>::min(const Coord<T>& coord) const
{
    return Coord<T>(std::min(x, coord.x), std::min(y, coord.y));
}

// _x = max(x, x')
// _y = max(y, y')
template <typename T>
Coord<T> Coord<T>::max(const Coord<T>& coord) const
{
    return Coord<T>(std::max(x, coord.x), std::max(y, coord.y));
}


// produit scalaire : x * x' + y * y'
template <typename T>
T Coord<T>::scalar(const Coord<T>& coord) const
{
    return x * coord.x + y * coord.y;
}

// determinant : x * y' - y * x'
template <typename T>
T Coord<T>::det(const Coord<T>& coord) const
{
    return x * coord.y - y * coord.x;
}

// coefficient directeur : y / x
template <typename T>
T Coord<T>::coeffDir() const
{
    return y / x;
}

// inverse du coefficient directeur : x / y
template <typename T>
T Coord<T>::invCoeffDir() const
{
    return x / y;
}

// x * y
template <typename T>
T Coord<T>::product() const
{
    return x * y;
}

// _x = x * x' - y * y'
// _y = x * y' + y * x'
template <typename T>
Coord<T> Coord<T>::complexProduct(const Coord<T>& coord) const
{
    return Coord<T>(x * coord.x - y * coord.y, x * coord.y + y * coord.x);
}

// racine carrée complexe :
template <typename T>
Coord<T> Coord<T>::complexSqrt() const
{
    //*
    double arg = this->atan() / 2;
    double mod = this->length();
    return Coord<T>(T(mod * cos(arg)), T(mod * sin(arg)));
    /*/
    double _x = sqrt((1 + this->cos()) / 2) * length();
    double _y;
    if (y < 0)
        _y = -sqrt((1 - this->cos()) / 2) * length();
    else
        _y = sqrt((1 - this->cos()) / 2) * length();
    return Coord<T>(T(_x), T(_y));
    //*/
}

// conjugué complexe
template <typename T>
Coord<T> Coord<T>::conj() const
{
    return Coord<T>(x, -y);
}

// x^2 + y^2, i.e. module au carré
template <typename T>
double Coord<T>::squareLength() const
{
    return x * x + y * y;
}

// longueur du vecteur, i.e. module
template <typename T>
double Coord<T>::length() const
{
    return sqrt(this->squareLength());
}

// arctangente, i.e. argument
template <typename T>
double Coord<T>::atan() const
{
    return atan2(y, x);
}

// cosinus de l'argument, i.e. x / length()
template <typename T>
double Coord<T>::cos() const
{
    return x / this->length();
}

// sinus de l'argument, i.e. y / length()
template <typename T>
double Coord<T>::sin() const
{
    return y / this->length();
}

#endif // COORD_TPL
