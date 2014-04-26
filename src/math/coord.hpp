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

#ifndef COORD_HPP
#define COORD_HPP

// la class Coord décrit un vecteur de dimension 2
template <typename T>
class Coord
{
public:
    class Iterator
    {
    public:
        // constructeur : coordonnées de départ et d'arrivée
        Iterator(const Coord<T>& begin, const Coord<T>& end);

        // incrémente l'itérateur
        Iterator& operator++()
        {
            ++mCurrent.x;

            if (mCurrent.x >= mEnd.x)
            {
                mCurrent.x = mBegin.x;
                ++mCurrent.y;
                mNewLine = true;

                if (mCurrent.y >= mEnd.y)
                    mActive = false;
            }

            return *this;
        }

        // déréférence l'itérateur
        const Coord<T>& operator*() const;
        const Coord<T>* operator->() const;

        // vérifie si l'itérateur est actif, s'il entre dans une nouvelle ligne
        bool active() const;
        bool new_line();

    private:
        // coordonnées actuelles
        Coord<T> mCurrent;
        // coordonnées de référence
        Coord<T> mBegin;
        Coord<T> mEnd;

        // booléen qui indique la fin du parcours
        bool mActive;
        // booléen qui indique l'entrée dans une nouvelle ligne
        bool mNewLine;
    };

    // vecteur nul
    Coord();
    // vecteur
    Coord(const T& X, const T& Y);
    // y = x
    explicit Coord(const T& X);

    // transtypage
    template <typename _T>
    explicit Coord(const Coord<_T>& coord);

    // x == y ?
    bool isSquare() const;
    // compare les vecteurs
    bool operator==(const Coord<T>& coord) const;
    bool operator!=(const Coord<T>& coord) const;

    // ordre lexicographique
    bool operator<(const Coord<T>& coord) const;

    // addition de vecteurs
    void operator+=(const Coord<T>& coord);
    void operator+=(const T& move);
    Coord<T> operator+(const Coord<T>& coord) const;
    Coord<T> operator+(const T& move) const;
    // soustraction de vecteurs
    void operator-=(const Coord<T>& coord);
    Coord<T> operator-(const Coord<T>& coord) const;
    Coord<T> operator-() const;
    // multiplie chaque composante du vecteur par la même composante de l'autre vecteur
    void operator*=(const Coord<T>& coord);
    Coord<T> operator*(const Coord<T>& coord) const;
    // multiplie chaque composante du vecteur par le ratio
    void operator*=(const T& ratio);
    Coord<T> operator*(const T& ratio) const;
    // divise chaque composante du vecteur par la même composante de l'autre vecteur
    void operator/=(const Coord<T>& coord);
    Coord<T> operator/(const Coord<T>& coord) const;
    // divise chaque composante du vecteur par le ratio
    void operator/=(const T& ratio);
    Coord<T> operator/(const T& ratio) const;

    // _x = min(x, x')
    // _y = min(y, y')
    Coord<T> min(const Coord<T>& coord) const;
    // _x = max(x, x')
    // _y = max(y, y')
    Coord<T> max(const Coord<T>& coord) const;

    // produit scalaire : x * x' + y * y'
    T scalar(const Coord<T>& coord) const;
    // determinant : x * y' - y * x'
    T det(const Coord<T>& coord) const;
    // coefficient directeur : y / x
    T coeffDir() const;
    // inverse du coefficient directeur : x / y
    T invCoeffDir() const;
    // x * y
    T product() const;
    // _x = x * x' - y * y'
    // _y = x * y' + y * x'
    Coord<T> complexProduct(const Coord<T>& coord) const;
    // racine carrée complexe :
    // _x = sqrt((1 + cos(x)) / 2) * length()
    // _y = sqrt((1 - cos(x)) / 2) * length()
    Coord<T> complexSqrt() const;
    // conjugué complexe
    Coord<T> conj() const;
    // x^2 + y^2, i.e. module au carré
    double squareLength() const;
    // longueur du vecteur, i.e. module
    double length() const;
    // arctangente, i.e. argument
    double atan() const;
    // cosinus, i.e. x / length()
    double cos() const;
    // sinus, i.e. y / length()
    double sin() const;

    // les 2 composantes du vecteur
    T x;
    T y;
};

#endif // COORD_HPP
