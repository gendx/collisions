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

#ifndef PROFIL_HPP
#define PROFIL_HPP

#include <QColor>
#include <QMap>
#include <QList>

#include "time.hpp"
#include "config_profil.hpp"

// Classe pour stocker les valeurs d'une profil.
class Profil
{
public:
    // Constructeur.
    inline Profil(const ConfigProfil& config);

    // Calcule et ajoute une tranche au profil.
    void push(State& state);

    // Accesseurs.
    inline const QList<std::pair<Time, QMap<int, double> > >& valeurs() const;

    Time maxTime() const;
    Time minTime() const;
    int minSlice() const;
    int maxSlice() const;
    double max() const;

private:
    // Ajoute une tranche au profil.
    void push(Time time, const QMap<int, double>& valeur);

    // Valeurs du profil.
    QList<std::pair<Time, QMap<int, double> > > mValeurs;
    // Configuration.
    ConfigProfil mConfig;
};

// Constructeur.
inline Profil::Profil(const ConfigProfil& config) :
    mConfig(config) {}

// Accesseurs.
inline const QList<std::pair<Time, QMap<int, double> > >& Profil::valeurs() const
    {return mValeurs;}

#endif // PROFIL_HPP
