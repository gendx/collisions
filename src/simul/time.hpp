/*
    Collisions - a real-time simulation program of colliding particles.
    Copyright (C) 2011 - 2013  G. Endignoux

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

#ifndef TIME_HPP
#define TIME_HPP

#include <iostream>

// Classe pour gérer un moment dans le temps.
class Time
{
public:
    // Affichage dans un flux standard.
    friend std::ostream& operator<<(std::ostream& flux, const Time& time);

    // Constructeurs.
    Time(); // Jamais.
    Time(double time);
    Time(const Time& time);

    // Opérateurs.
    void operator=(const Time& time);
    void operator-=(const Time& time);
    Time operator-(const Time& time) const;
    void operator+=(const Time& time);
    Time operator+(const Time& time) const;

    // Comparaison.
    bool operator<(const Time& time) const;
    inline bool operator<=(const Time& time) const;
    bool operator==(const Time& time) const;
    inline bool operator!=(const Time& time) const;

    // Accesseurs.
    inline bool isNever() const;
    inline double time() const;

private:
    // Temps virtuel (dans le référentiel de la simulation) depuis le début de la simulation.
    double mTime;
    // Jamais.
    bool mNever;
};

// Comparaison.
//*
bool Time::operator<=(const Time& time) const
    {return !(time < *this);}//*/
bool Time::operator!=(const Time& time) const
    {return !(time == *this);}

// Accesseurs.
bool Time::isNever() const
    {return mNever;}
double Time::time() const
    {return mTime;}

#endif // TIME_HPP
