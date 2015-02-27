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

#ifndef STATE_HPP
#define STATE_HPP

#include <QTime>
#include "population.hpp"
#include "boule.hpp"
#include "piston.hpp"
#include "collision.hpp"
#include "obstacle.hpp"
#include "map_ligne.hpp"

// Classe représentant l'état de la simulation.
class State
{
public:
    // Constructeur.
    State(const Configuration& cfg);

    // Vide l'état actuel.
    void clear();
    // Construit une nouvelle simulation à partir de la configuration.
    void create();

private:
    // Ajoute des éléments à la simulation.
    void addObstacles();
    void addObstacle(const Polygone& sommets);
    void addSegment(const Segment& segment);

public:
    // Configuration et objets de la simulation.
    const Configuration& config;
    std::vector<Population> populations;
    std::vector<std::shared_ptr<Boule> > boules;
    std::vector<std::shared_ptr<Piston> > pistons;
    std::map<int, MapLigne> mapMobiles;
    Time now;
    double sizeArea;

    // Evénements à simuler.
    std::multimap<Time, std::shared_ptr<Event> > events;
    std::set<Mobile*> toRefresh;
    std::list<std::multimap<Time, std::shared_ptr<Event> >::iterator> drawingsRefresh;

    // Fréquences d'affichage.
    Time stepDraw;
    Time stepValues;
    Time stepCourbes;
    // Statistiques.
    unsigned int countChocs;
    std::pair<unsigned int, unsigned int> countEtudes;
    unsigned int totalEtudes;
    std::list<std::pair<QTime, unsigned int> > frames;
};

#endif // STATE_HPP
