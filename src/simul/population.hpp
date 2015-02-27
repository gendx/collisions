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
    inline std::set<Boule*>& boules();
    inline const std::set<Boule*>& boules() const;

    // Calcule des statistiques sur cette population.
    double meanFreeRide() const;
    double meanFreeTime() const;
    double meanFromOrigin() const;
    double squareFromOrigin() const;
    double totalVitesse() const;
    double totalVit2() const;
    inline double meanVitesse() const;
    inline double meanVit2() const;
    inline double meanEnergy() const;
    inline double meanPressure() const;
    inline double totalEnergy() const;
    //inline double totalPressure() const;
    /*
    QMap<int, double> profilPressure(double slice) const;
    QMap<int, double> profilTemperature(double slice) const;//*/
    inline QColor color() const;

private:
    bool invalid(const Coord<double>& pos, State& state);

    ConfigPopulation mConfig;
    std::set<Boule*> mBoules;
};

// Constructeurs.
inline Population::Population() :
    mConfig() {}
inline Population::Population(const ConfigPopulation& config) :
    mConfig(config) {}

// Accesseurs.
inline std::set<Boule*>& Population::boules()
    {return mBoules;}
inline const std::set<Boule*>& Population::boules() const
    {return mBoules;}

// Calcule des statistiques sur cette population.
inline double Population::meanVitesse() const
    {return this->totalVitesse() / mConfig.mTaille;}
inline double Population::meanVit2() const
    {return this->totalVit2() / mConfig.mTaille;}
inline double Population::meanEnergy() const
    {return this->meanVit2() * mConfig.mMasse;}
inline double Population::meanPressure() const
    {double freeRide = this->meanFreeRide(); return freeRide ? (this->meanEnergy() / (freeRide * freeRide)) : 0;}
inline double Population::totalEnergy() const
    {return this->totalVit2() * mConfig.mMasse;}/*
inline double Population::totalPressure() const
    {double freeRide = this->meanFreeRide(); if (freeRide) return this->totalEnergy() / freeRide; else return 0;}*/
inline QColor Population::color() const
    {return mConfig.mColor;}

#endif // POPULATION_HPP
