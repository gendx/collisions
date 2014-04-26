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

#ifndef COURBE_HPP
#define COURBE_HPP

#include <QList>
#include <QColor>
#include "time.hpp"
#include "config_courbe.hpp"

// Classe pour stocker les valeurs d'une courbe.
class Courbe
{
public:
    // Constructeur.
    inline Courbe(const ConfigCourbe& config);

    // Calcule et ajoute une valeur à la courbe.
    void push(Time time, unsigned int valType, bool mean, const QList<Population>& populations, const QList<std::shared_ptr<Piston> >& pistons);

    // Accesseurs.
    inline const QList<std::pair<Time, double> >& valeurs() const;
    inline QColor color() const;

    Time maxTime() const;
    double max() const;
    double min() const;

private:
    // Ajoute une valeur à la courbe.
    void push(Time time, double valeur);

    // Valeurs de la courbe.
    QList<std::pair<Time, double> > mValeurs;
    // Configuration.
    ConfigCourbe mConfig;
};

// Constructeur.
inline Courbe::Courbe(const ConfigCourbe& config) :
    mConfig(config) {}

// Accesseurs.
inline const QList<std::pair<Time, double> >& Courbe::valeurs() const
    {return mValeurs;}
inline QColor Courbe::color() const
    {return mConfig.mColor;}

#endif // COURBE_HPP
