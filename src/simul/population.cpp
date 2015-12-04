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

#include "population.hpp"

#include <iostream>
#include <limits>
#include "coord_io.tpl"
#include "state.hpp"

// Génère une population de boules selon la configuration.
void Population::create(unsigned int index, State& state)
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
        while (this->invalid(pos, state));

        // Ajoute une boule.
        std::unique_ptr<Boule> boule = std::make_unique<Boule>(
                    pos,
                    Coord<double>(distribVitesse(Solveur::generateur),
                                  distribVitesse(Solveur::generateur)),
                    mConfig.mColor,
                    mConfig.mMasse,
                    mConfig.mRayon,
                    state);
        state.boules.push_back(std::move(boule));

        state.boules.back()->setPopulation(index, state);
        state.boules.back()->updateCollisions(state);
    }
}

// Vérifie les contraintes d'intersection entre les différents objets.
bool Population::invalid(const Coord<double>& pos, State& state)
{
    // Vérifie que la boule est bien dans la zone autorisée.
    auto& contour = state.config.contour().sommets();
    if ((mConfig.mPolygone.intersect(pos, mConfig.mRayon) || !mConfig.mPolygone.inside(pos))
        || (contour.intersect(pos, mConfig.mRayon)
        || !contour.inside(pos)))
        return true;

    // Vérifie les intersections avec les obstacles.
    for (auto& obstacle : state.config.obstacles())
    {
        auto& sommets = obstacle.sommets();
        if (sommets.intersect(pos, mConfig.mRayon)
         || sommets.inside(pos))
            return true;
    }

    // Vérifie les intersections avec les pistons.
    for (auto& piston : state.pistons)
        if ((piston->position().y - pos.y) <= mConfig.mRayon
         && (pos.y - piston->position().y) <= mConfig.mRayon + piston->epaisseur())
            return true;

    // Vérifie les intersections avec les autres boules.
    for (auto& boule : state.boules)
        if ((boule->position() - pos).length() <= mConfig.mRayon + boule->rayon())
            return true;

    return false;
}
