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

#include "population.hpp"

#include <iostream>
#include <limits>
#include "coord_io.tpl"
#include "piston.hpp"

// Génère une population de boules selon la configuration.
void Population::create(const Configuration& config, QList<Population>& populations, unsigned int index, QMap<int, MapLigne>& mapMobiles, QList<std::shared_ptr<Piston> >& pistons, std::multimap<Time, std::shared_ptr<Event> >& events, const Time& now, double sizeArea)
{
    // Distributions aléatoires en position (uniforme) et en vitesse (normale sur chaque axe).
    std::uniform_real_distribution<> distribX(mConfig.mPolygone.left(), mConfig.mPolygone.right());
    std::uniform_real_distribution<> distribY(mConfig.mPolygone.top(), mConfig.mPolygone.bottom());
    std::normal_distribution<> distribVitesse(0, mConfig.mVitesse);

    for (unsigned int i = 0 ; i < mConfig.mTaille ; ++i)
    {
        // Crée une position valable.
        Coord<double> pos;

        do
            pos = Coord<double>(distribX(Solveur::generateur), distribY(Solveur::generateur));
        while (this->invalid(pos, config, populations, pistons));

        // Ajoute une boule.
        auto it = mBoules.insert(mBoules.end(),
                                 std::make_shared<Boule>(
                                     pos,
                                     Coord<double>(distribVitesse(Solveur::generateur),
                                                   distribVitesse(Solveur::generateur)),
                                     mConfig.mColor,
                                     mConfig.mMasse,
                                     mConfig.mRayon,
                                     now,
                                     sizeArea,
                                     mapMobiles)
                                 );
        mBoules.back()->setPopulation(now, index, it, events, config.configMutations());
        std::pair<unsigned int, unsigned int> countEtudes;
        mBoules.back()->updateCollisions(events, mapMobiles, now, sizeArea, config.gravity(), countEtudes);
    }
}

// Vérifie les contraintes d'intersection entre les différents objets.
bool Population::invalid(const Coord<double>& pos, const Configuration& config, QList<Population>& populations, QList<std::shared_ptr<Piston> >& pistons)
{
    // Vérifie que la boule est bien dans la zone autorisée.
    if ((mConfig.mPolygone.intersect(pos, mConfig.mRayon) || !mConfig.mPolygone.inside(pos))
        || (config.contour().sommets().intersect(pos, mConfig.mRayon)
        || !config.contour().sommets().inside(pos)))
        return true;

    // Vérifie les intersections avec les obstacles.
    for (auto& obstacle : config.obstacles())
    {
        auto& sommets = obstacle.sommets();
        if (sommets.intersect(pos, mConfig.mRayon)
         || sommets.inside(pos))
            return true;
    }

    // Vérifie les intersections avec les pistons.
    for (auto& piston : pistons)
        if ((piston->position().y - pos.y) <= mConfig.mRayon
         && (pos.y - piston->position().y) <= mConfig.mRayon + piston->epaisseur())
            return true;

    // Vérifie les intersections avec les autres boules de cette population.
    for (auto& boule : mBoules)
        if ((boule->position() - pos).length() <= 2 * mConfig.mRayon)
            return true;

    // Vérifie les intersections avec les boules des autres populations.
    for (auto& population : populations)
        for (auto& boule : population.mBoules)
            if ((boule->position() - pos).length() <= mConfig.mRayon + population.mConfig.mRayon)
                return true;

    return false;
}

// Calcule le libre parcours moyen.
double Population::meanFreeRide() const
{
    double result = 0;
    unsigned int nbre = 0;
    for (auto& boule : mBoules)
    {
        if (boule->validFree())
        {
            result += boule->freeRide().length();
            ++nbre;
        }
    }

    if (nbre > mBoules.size() * .9)
        return result / nbre;
    return std::numeric_limits<double>::quiet_NaN();
}

// Calcule le temps moyen de libre parcours.
double Population::meanFreeTime() const
{
    double result = 0;
    unsigned int nbre = 0;
    for (auto& boule : mBoules)
    {
        if (boule->validFree())
        {
            result += boule->freeTime().time();
            ++nbre;
        }
    }

    if (nbre > mBoules.size() * .9)
        return result / nbre;
    return std::numeric_limits<double>::quiet_NaN();
}

// Calcule la distance moyenne parcourue depuis le début.
double Population::meanFromOrigin() const
{
    double result = 0;
    for (auto& boule : mBoules)
        result += boule->fromOrigin().length();
    return result / mBoules.size();
}

// Calcule la moyenne du carré de la distance parcourue depuis le début.
double Population::squareFromOrigin() const
{
    double result = 0;
    for (auto& boule : mBoules)
        result += boule->fromOrigin().squareLength();
    return result / mBoules.size();
}

// Calcule la somme des vitesses.
double Population::totalVitesse() const
{
    double result = 0;
    for (auto& boule : mBoules)
        result += boule->vitesse().length();
    return result;
}

// Calcule la somme des carrés des vitesses.
double Population::totalVit2() const
{
    double result = 0;
    for (auto& boule : mBoules)
        result += boule->vitesse().squareLength();
    return result;
}
