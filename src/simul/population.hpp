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

#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <memory>
#include "boule.hpp"
#include "polygone.hpp"
#include "obstacle.hpp"
#include "configuration.hpp"
#include "config_population.hpp"
#include "solveur.hpp"

// Population de boules partageant les mêmes caractéristiques (couleur, masse, taille).
class Population
{
public:
    // Constructeurs.
    inline Population();
    inline Population(const ConfigPopulation& config);

    // Génère une population de boules selon la configuration.
    void create(unsigned int index, State& state);

    // Accesseurs.
    inline QColor color() const;

private:
    bool invalid(const Coord<double>& pos, State& state);

    ConfigPopulation mConfig;
    //std::set<Boule*> mBoules;
};

// Constructeurs.
inline Population::Population() :
    mConfig() {}
inline Population::Population(const ConfigPopulation& config) :
    mConfig(config) {}

// Accesseurs.
inline QColor Population::color() const
    {return mConfig.mColor;}

#endif // POPULATION_HPP
