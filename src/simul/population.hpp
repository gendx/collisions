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

#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <QMap>
#include <memory>
#include <list>
#include "boule.hpp"
#include "polygone.hpp"
#include "obstacle.hpp"
#include "configuration.hpp"
#include "config_population.hpp"
#include "solveur.hpp"

class Population
{
public:
    Population();
    Population(const ConfigPopulation& config);

    void create(const Configuration& config, QList<Population>& populations, unsigned int index, QMap<int, MapLigne>& mapMobiles, QList<std::shared_ptr<Piston> >& pistons, std::multimap<Time, std::shared_ptr<Event> >& events, const Time& now, double sizeArea);

    inline std::list<std::shared_ptr<Boule> >& boules();
    inline const std::list<std::shared_ptr<Boule> >& boules() const;
    double meanFreeRide() const;
    double meanFreeTime() const;
    double meanFromOrigin() const;
    double squareFromOrigin() const;
    inline double meanVitesse() const;
    inline double meanVit2() const;
    inline double meanEnergy() const;
    inline double meanPressure() const;
    double totalVitesse() const;
    double totalVit2() const;
    inline double totalEnergy() const;
    ///inline double totalPressure() const;
    /*
    QMap<int, double> profilPressure(double slice) const;
    QMap<int, double> profilTemperature(double slice) const;//*/
    inline QColor color() const;

private:
    bool invalid(const Coord<double>& pos, const Configuration& config, QList<Population>& populations, QList<std::shared_ptr<Piston> >& pistons);

    ConfigPopulation mConfig;
    std::list<std::shared_ptr<Boule> > mBoules;
};

inline Population::Population() :
    mConfig() {}
inline Population::Population(const ConfigPopulation& config) :
    mConfig(config) {}

inline std::list<std::shared_ptr<Boule> >& Population::boules()
    {return mBoules;}
inline const std::list<std::shared_ptr<Boule> >& Population::boules() const
    {return mBoules;}
inline double Population::meanVitesse() const
    {return this->totalVitesse() / mConfig.mTaille;}
inline double Population::meanVit2() const
    {return this->totalVit2() / mConfig.mTaille;}
inline double Population::meanEnergy() const
    {return this->meanVit2() * mConfig.mMasse;}
inline double Population::meanPressure() const
    {double freeRide = this->meanFreeRide(); if (freeRide) return this->meanEnergy() / (freeRide * freeRide); else return 0;}
inline double Population::totalEnergy() const
    {return this->totalVit2() * mConfig.mMasse;}/*
inline double Population::totalPressure() const
    {double freeRide = this->meanFreeRide(); if (freeRide) return this->totalEnergy() / freeRide; else return 0;}*/
inline QColor Population::color() const
    {return mConfig.mColor;}

#endif // POPULATION_HPP
