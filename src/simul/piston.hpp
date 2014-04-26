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

#ifndef PISTON_HPP
#define PISTON_HPP

#include "mobile.hpp"
#include "config_piston.hpp"

// Mobile décrivant un piston horizontal.
class Piston : public Mobile
{
    friend class Boule;

public:
    // Constructeur.
    Piston(ConfigPiston config, double sizeArea, QMap<int, MapLigne>& mapMobiles);

    // Accesseurs.
    inline double epaisseur() const;

    // Calcule l'instant de la prochaine collision avec le mobile.
    Time collision(const Mobile* mobile) const;
    Time collision(const Boule* boule) const;
    Time collision(const Piston* piston) const;
    // Calcule l'instant du prochain changement de zone.
    Time newArea(double sizeArea, const Coord<double>& gravity) const;

    // Effectue la collision avec le mobile.
    void doCollision(const Time& now, Mobile* mobile, std::set<Mobile*>& toRefresh, std::multimap<Time, std::shared_ptr<Event> >& events, const QList<ConfigMutation>& configMutations, const QList<ConfigReaction>& configReactions, QList<Population>& populations, std::pair<unsigned int, unsigned int>& countEtudes);
    void doCollision(const Time& now, Boule* boule, std::set<Mobile*>& toRefresh, std::multimap<Time, std::shared_ptr<Event> >& events, const QList<ConfigMutation>& configMutations, const QList<ConfigReaction>& configReactions, QList<Population>& populations, std::pair<unsigned int, unsigned int>& countEtudes);
    void doCollision(const Time& now, Piston* piston, std::set<Mobile*>& toRefresh, std::multimap<Time, std::shared_ptr<Event> >& events, const QList<ConfigMutation>& configMutations, const QList<ConfigReaction>& configReactions, QList<Population>& populations, std::pair<unsigned int, unsigned int>& countEtudes);
    void changeArea(double sizeArea, std::set<Mobile*>& toRefresh, QMap<int, MapLigne>& mapMobiles, std::pair<unsigned int, unsigned int>& countEtudes);

private:
    // Cherche des collisions avec des mobiles.
    void updateCollisionsMobiles(std::multimap<Time, std::shared_ptr<Event> >& events, QMap<int, MapLigne>& mapMobiles, const Time& now, double sizeArea, const Coord<double>& gravity, std::pair<unsigned int, unsigned int>& countEtudes);

    // Propriétés géométriques.
    double mEpaisseur;
    int mArea1;
    int mArea2;
    QList<Piston*>::iterator mMapIt1;
    QList<Piston*>::iterator mMapIt2;
};

// Accesseurs.
inline double Piston::epaisseur() const
    {return mEpaisseur;}

#endif // PISTON_HPP
