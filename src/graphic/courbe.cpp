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

#include "courbe.hpp"

// Calcule et ajoute une valeur à la courbe.
void Courbe::push(Time time, unsigned int valType, bool mean, const QList<Population>& populations, const QList<boost::shared_ptr<Piston> >& pistons)
{
    unsigned int nbre = 0;
    double valeur = 0;

    for (int i = 0 ; i < mConfig.mCibles.size() ; ++i)
        valeur += mConfig.mCibles[i].value(valType, populations, mConfig.mPolygone, pistons, nbre);

    if (mean)
        valeur /= nbre;
    this->push(time, valeur);
}

// Ajoute une valeur à la courbe.
void Courbe::push(Time time, double valeur)
{
    if (!(valeur == valeur))
        return;

    mValeurs.append(std::make_pair(time, valeur));
}

// Calculs de minima et maxima.
Time Courbe::maxTime() const
{
    if (mValeurs.isEmpty())
        return 0;

    Time max = mValeurs[0].first;
    for (int i = 1 ; i < mValeurs.size() ; ++i)
        if (max < mValeurs[i].first)
            max = mValeurs[i].first;
    return max;
}

double Courbe::max() const
{
    if (mValeurs.isEmpty())
        return 0;

    double max = mValeurs[0].second;
    for (int i = 1 ; i < mValeurs.size() ; ++i)
        if (mValeurs[i].second > max)
            max = mValeurs[i].second;
    return max;
}

double Courbe::min() const
{
    if (mValeurs.isEmpty())
        return 0;

    double min = mValeurs[0].second;
    for (int i = 1 ; i < mValeurs.size() ; ++i)
        if (mValeurs[i].second < min)
            min = mValeurs[i].second;
    return min;
}
