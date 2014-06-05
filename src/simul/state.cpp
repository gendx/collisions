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

#include "state.hpp"

// Constructeur.
State::State(const Configuration& cfg) :
    config(cfg),
    sizeArea(),
    countChocs(0),
    countEtudes(0, 0),
    totalEtudes(0)
{
}


// Vide l'état actuel.
void State::clear()
{
    events.clear();
    toRefresh.clear();
    drawingsRefresh.clear();
    populations.clear();
    pistons.clear();
    mapMobiles.clear();
    now = 0;
    countChocs = 0;
    countEtudes.first = 0;
    countEtudes.second = 0;
    totalEtudes = 0;
    frames.clear();
}

// Construit une nouvelle simulation à partir de la configuration.
void State::create()
{
    sizeArea = config.sizeArea();

    // Création des obstacles.
    this->addObstacles();

    // Création des pistons.
    for (auto& piston : config.configPistons())
    {
        pistons.push_back(std::make_shared<Piston>(piston, *this));
        pistons.back()->updateCollisions(*this);
    }

    // Création des populations.
    const auto& configPops = config.configPops();
    for (int i = 0 ; i < configPops.size() ; ++i)
    {
        populations.push_back(Population(configPops[i]));
        populations.back().create(i, *this);
    }
}


// Ajoute les obstacles à la simulation.
void State::addObstacles()
{
    // ajout des sommets
    for (auto& obstacle : config.obstacles())
        this->addObstacle(obstacle.sommets());
    this->addObstacle(config.contour().sommets());
}

// Ajoute un obstacle à la simulation.
void State::addObstacle(const Polygone& sommets)
{
    for (unsigned int j = 0 ; j < sommets.size() ; ++j)
    {
        const Coord<double>& point = sommets.point(j);
        mapMobiles[std::floor(point.y / sizeArea)].sommets().insert(std::floor(point.x / sizeArea), point);
        this->addSegment(sommets.segment(j));
    }
}

// Ajoute un segment à la simulation.
void State::addSegment(const Segment& segment)
{
    // Extrémités du segment.
    Coord<int> point1 = segment.point1(sizeArea);
    Coord<int> point2 = segment.point2(sizeArea);

    // Rectangle contenant le segment.
    Coord<int> min = point1.min(point2);
    Coord<int> max = point1.max(point2);

    // Ajout aux zones intersectées.
    for (int i = min.x ; i < max.x ; ++i)
    {
        int y = std::floor(segment.yAtX(i * sizeArea) / sizeArea);
        mapMobiles[y].segments()[i].insert(segment);
        mapMobiles[y].segments()[i + 1].insert(segment);
    }

    for (int j = min.y ; j < max.y ; ++j)
    {
        int x = std::floor(segment.xAtY(j * sizeArea) / sizeArea);
        mapMobiles[j].segments()[x].insert(segment);
        mapMobiles[j + 1].segments()[x].insert(segment);
    }
}
