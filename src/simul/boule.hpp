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

#ifndef BOULE_HPP
#define BOULE_HPP

#include "mobile.hpp"

// Mobile décrivant une particule en forme de boule.
class Boule : public Mobile
{
public:
    // Constructeur.
    Boule(const Coord<double>& position, const Coord<double>& vitesse, const QColor& color, double masse, double rayon, State& state);

    // Avance le mobile jusqu'à l'instant indiqué (sans tenir compte des autres mobiles).
    virtual void avance(const Time& time, const Coord<double>& gravity);

    // Effectue une mutation pour cette boule.
    void changePopulation(State& state);
    // Définit la population de cette boule et prépare la prochaine mutation.
    void setPopulation(unsigned int population, State& state);

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
    virtual void doCollision(Mobile* mobile, State& state);
    virtual void doCollision(Boule* boule, State& state);
    virtual void doCollision(Piston* piston, State& state);
    virtual void doCollision(const Coord<double>& sommet, State& state);
    virtual void doCollision(const Segment& segment, State& state);
    void changeArea(State& state);

    // Recalcule la zone et met à jour la table des zones.
    void setArea(State& state);

private:
    // Cherche des collisions avec des mobiles.
    void updateCollisionsMobiles(State& state);
    // Enlève la boule de la table des zones.
    void detachArea(State& state);

    // Change la boule de population.
    void swap(unsigned int population, State& state, bool eraseEvent);

    // Propriétés géométriques.
    Coord<double> mOrigine;
    std::pair<Coord<double>, Time> mOldFree;
    std::pair<Coord<double>, Time> mLastFree;
    double mRayon;
    Coord<int> mArea;
    QMultiMap<int, Boule*>::iterator mMapIt;

    // Population contenant la boule.
    unsigned int mPopulation;
    std::multimap<Time, std::shared_ptr<Event> >::iterator mEventIt;
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
