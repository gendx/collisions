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

#include "profil.hpp"

#include <limits>

// Calcule et ajoute une tranche au profil.
void Profil::push(Time time, const QList<Population>& populations)
{
    QMap<int, unsigned int> nbres;
    QMap<int, double> valeurs;

    for (int i = 0 ; i < mConfig.mCibles.size() ; ++i)
        mConfig.mCibles[i].addValue(mConfig.mType, populations, mConfig.mPolygone, valeurs, nbres, mConfig.mSlice);

    if (mConfig.mMean)
        for (auto it = valeurs.begin() ; it != valeurs.end() ; ++it)
            it.value() /= nbres[it.key()];
    this->push(time, valeurs);
}

// Ajoute une tranche au profil.
void Profil::push(Time time, const QMap<int, double>& valeur)
{
    mValeurs.append(std::make_pair(time, valeur));
}

// Calculs de minima et maxima.
Time Profil::maxTime() const
{
    if (mValeurs.isEmpty())
        return 0;

    Time max = mValeurs[0].first;
    for (int i = 1 ; i < mValeurs.size() ; ++i)
        if (max < mValeurs[i].first)
            max = mValeurs[i].first;
    return max;
}

Time Profil::minTime() const
{
    if (mValeurs.isEmpty())
        return 0;

    Time min = mValeurs[0].first;
    for (int i = 1 ; i < mValeurs.size() ; ++i)
        if (mValeurs[i].first < min)
            min = mValeurs[i].first;
    return min;
}

// Calcule la valeur maximale des données du profil.
double Profil::max() const
{
    if (mValeurs.isEmpty())
        return 0;

    double max = std::numeric_limits<double>::quiet_NaN();
    for (int i = 0 ; i < mValeurs.size() ; ++i)
        for (auto it = mValeurs[i].second.begin() ; it != mValeurs[i].second.end() ; ++it)
            if (it.value() > max || (!(max == max)))
                max = it.value();

    return max;
}

// Calcule les positions verticales extrémales du profil.
int Profil::maxSlice() const
{
    if (mValeurs.isEmpty())
        return 0;

    float max = std::numeric_limits<float>::quiet_NaN();
    for (int i = 0 ; i < mValeurs.size() ; ++i)
    {
        auto it = mValeurs[i].second.end();
        --it;
        if (it.key() > max || (!(max == max)))
            max = it.key();
    }
    return (int)max;
}

int Profil::minSlice() const
{
    if (mValeurs.isEmpty())
        return 0;

    float min = std::numeric_limits<float>::quiet_NaN();
    for (int i = 0 ; i < mValeurs.size() ; ++i)
    {
        auto it = mValeurs[i].second.begin();
        if (it.key() < min || (!(min == min)))
            min = it.key();
    }
    return (int)min;
}
