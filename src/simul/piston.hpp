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

#ifndef PISTON_HPP
#define PISTON_HPP

#include "mobile.hpp"
#include "config_piston.hpp"

// Mobile décrivant un piston horizontal.
class Piston : public Mobile
{
    friend class Boule;

public:
    // Constructeur.
    Piston(ConfigPiston config, State& state);

    // Accesseurs.
    inline double epaisseur() const;

    // Calcule l'instant de la prochaine collision avec le mobile.
    Time collision(const Mobile* mobile) const;
    Time collision(const Boule* boule) const;
    Time collision(const Piston* piston) const;
    // Calcule l'instant du prochain changement de zone.
    Time newArea(double sizeArea, const Coord<double>& gravity) const;

    // Effectue la collision avec le mobile.
    void doCollision(Mobile* mobile, State& state);
    void doCollision(Boule* boule, State& state);
    void doCollision(Piston* piston, State& state);
    void changeArea(State& state);

private:
    // Cherche des collisions avec des mobiles.
    void updateCollisionsMobiles(State& state);

    // Propriétés géométriques.
    double mEpaisseur;
    int mArea1;
    int mArea2;
    QList<Piston*>::iterator mMapIt1;
    QList<Piston*>::iterator mMapIt2;
};

// Accesseurs.
inline double Piston::epaisseur() const
    {return mEpaisseur;}

#endif // PISTON_HPP
