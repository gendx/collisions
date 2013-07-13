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

#ifndef BOULE_HPP
#define BOULE_HPP

#include "mobile.hpp"

// Mobile décrivant une particule en forme de boule.
class Boule : public Mobile
{
public:
    // Constructeur.
    Boule(const Coord<double>& position, const Coord<double>& vitesse, const QColor& color, double masse, double rayon, const Time& now, double sizeArea, QMap<int, MapLigne>& mapMobiles);

    // Avance le mobile jusqu'à l'instant indiqué (sans tenir compte des autres mobiles).
    virtual void avance(const Time& time, const Coord<double>& gravity);

    // Effectue une mutation pour cette boule.
    void changePopulation(const Time& now, QList<Population>& populations, std::multimap<Time, boost::shared_ptr<Event> >& events, const QList<ConfigMutation>& configMutations);
    // Définit la population de cette boule et prépare la prochaine mutation.
    void setPopulation(const Time& now, unsigned int population, std::list<boost::shared_ptr<Boule> >::iterator iterator, std::multimap<Time, boost::shared_ptr<Event> >& events, const QList<ConfigMutation>& configMutations);

    // Accesseurs.
    inline Coord<double> freeRide() const;
    inline Time freeTime() const;
    inline bool validFree() const;
    inline Coord<double> fromOrigin() const;
    inline double rayon() const;
    inline Coord<int> area() const;

    // Calcule l'instant de la prochaine collision avec le mobile.
    Time collision(const Mobile* mobile) const;
    Time collision(const Boule* boule) const;
    Time collision(const Piston* piston) const;
    // Calcule l'instant de la prochaine collision avec l'obstacle.
    Time collision(const Coord<double>& sommet, const Coord<double>& gravity) const;
    Time collision(const Segment& segment, const Coord<double>& gravity) const;
    // Calcule l'instant du prochain changement de zone.
    Time newArea(double sizeArea, const Coord<double>& gravity) const;

    // Effectue la collision avec le mobile.
    void doCollision(const Time& now, Mobile* mobile, std::set<Mobile*>& toRefresh, std::multimap<Time, boost::shared_ptr<Event> >& events, const QList<ConfigMutation>& configMutations, const QList<ConfigReaction>& configReactions, QList<Population>& populations, std::pair<unsigned int, unsigned int>& countEtudes);
    void doCollision(const Time& now, Boule* boule, std::set<Mobile*>& toRefresh, std::multimap<Time, boost::shared_ptr<Event> >& events, const QList<ConfigMutation>& configMutations, const QList<ConfigReaction>& configReactions, QList<Population>& populations, std::pair<unsigned int, unsigned int>& countEtudes);
    void doCollision(const Time& now, Piston* piston, std::set<Mobile*>& toRefresh, std::multimap<Time, boost::shared_ptr<Event> >& events, const QList<ConfigMutation>& configMutations, const QList<ConfigReaction>& configReactions, QList<Population>& populations, std::pair<unsigned int, unsigned int>& countEtudes);
    void doCollision(const Time& now, const Coord<double>& sommet, std::set<Mobile*>& toRefresh, std::pair<unsigned int, unsigned int>& countEtudes);
    void doCollision(const Time& now, const Segment& segment, std::set<Mobile*>& toRefresh, std::pair<unsigned int, unsigned int>& countEtudes);
    void changeArea(double sizeArea, std::set<Mobile*>& toRefresh, QMap<int, MapLigne>& mapMobiles, std::pair<unsigned int, unsigned int>& countEtudes);

    // Recalcule la zone et met à jour la table des zones.
    void setArea(double sizeArea, std::set<Mobile*>& toRefresh, QMap<int, MapLigne>& mapMobiles);

private:
    // Cherche des collisions avec des mobiles.
    void updateCollisionsMobiles(std::multimap<Time, boost::shared_ptr<Event> >& events, QMap<int, MapLigne>& mapMobiles, const Time& now, double sizeArea, const Coord<double>& gravity, std::pair<unsigned int, unsigned int>& countEtudes);
    // Enlève la boule de la table des zones.
    void detachArea(QMap<int, MapLigne>& mapMobiles);

    // Change la boule de population.
    void swap(const Time& now, unsigned int population, QList<Population>& populations, std::multimap<Time, boost::shared_ptr<Event> >& events, const QList<ConfigMutation>& configMutations, bool eraseEvent);

    // Propriétés géométriques.
    Coord<double> mOrigine;
    std::pair<Coord<double>, Time> mOldFree;
    std::pair<Coord<double>, Time> mLastFree;
    double mRayon;
    Coord<int> mArea;
    QMultiMap<int, Boule*>::iterator mMapIt;

    // Population contenant la boule.
    unsigned int mPopulation;
    std::list<boost::shared_ptr<Boule> >::iterator mPopIt;
    std::multimap<Time, boost::shared_ptr<Event> >::iterator mEventIt;
};

// Accesseurs.
inline Coord<double> Boule::freeRide() const
    {return mLastFree.first - mOldFree.first;}
inline Time Boule::freeTime() const
    {return mLastFree.second - mOldFree.second;}
inline bool Boule::validFree() const
    {return !mOldFree.second.isNever();}
inline Coord<double> Boule::fromOrigin() const
    {return mPosition - mOrigine;}
inline double Boule::rayon() const
    {return mRayon;}
inline Coord<int> Boule::area() const
    {return mArea;}

#endif // BOULE_HPP
