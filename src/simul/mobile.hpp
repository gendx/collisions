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

#ifndef MOBILE_HPP
#define MOBILE_HPP

#include <QColor>
#include <QMultiMap>
#include <map>
#include <set>
#include <memory>

#include "coord.hpp"

#include "time.hpp"
#include "segment.hpp"
#include "polygone.hpp"

class State;
class Collision;
class Piston;
class Boule;
class Event;

// Comparaison pour utiliser un std::set.
bool operator<(const std::multimap<Time, std::shared_ptr<Event> >::iterator& it1, const std::multimap<Time, std::shared_ptr<Event> >::iterator& it2);

// Classe abstraite définissant un mobile (position, vitesse, masse, couleur).
class Mobile
{
public:
    // Affichage dans un flux standard.
    friend std::ostream& operator<<(std::ostream& flux, const Mobile& mobile);

    // Constructeur.
    Mobile(const Coord<double>& position, const Coord<double>& vitesse, const QColor& color, double masse);

    // Accesseurs.
    inline const Coord<double>& position() const;
    inline const Coord<double>& vitesse() const;
    inline double masse() const;
    inline QColor color() const;
    inline unsigned int id() const;

    // Affiche la liste des cibles dans la sortie standard.
    void showTargets() const;

    // Avance le mobile jusqu'à l'instant indiqué (sans tenir compte des autres mobiles).
    virtual void avance(const Time& time, const Coord<double>& gravity);

    // Calcule l'instant de la prochaine collision avec le mobile.
    virtual Time collision(const Mobile* mobile) const = 0;
    virtual Time collision(const Boule* boule) const = 0;
    virtual Time collision(const Piston* piston) const = 0;
    // Calcule l'instant de la prochaine collision avec l'obstacle.
    virtual Time collision(const Coord<double>& sommet, const Coord<double>& gravity) const;
    virtual Time collision(const Segment& segment, const Coord<double>& gravity) const;
    // Calcule l'instant du prochain changement de zone.
    virtual Time newArea(double sizeArea, const Coord<double>& gravity) const;

    // Effectue la collision avec le mobile : calcul du changement de trajectoire et mise à jour des prochaines collisions.
    virtual void doCollision(Mobile* mobile, State& state) = 0;
    virtual void doCollision(Boule* boule, State& state) = 0;
    virtual void doCollision(Piston* piston, State& state) = 0;
    virtual void doCollision(const Coord<double>& sommet, State& state);
    virtual void doCollision(const Segment& segment, State& state);
    // Effectue un changement de zone.
    virtual void changeArea(State& state);

    // Définit la dernière collision du mobile.
    void setLastCollision(const Collision& collision, const Time& now);
    // Vérifie que la collision est compatible avec la dernière collision du mobile.
    bool checkLastCollision(const Collision& collision, const Time& time) const;

    // Cherche la prochaine collision de ce mobile et met à jour la table des événements.
    void updateCollisions(State& state);
    // Détache le mobile de la collision (appelé en cas de changement de trajectoire).
    void detach(const Collision& collision);

protected:
    // Ajoute tous les mobiles attachés (i.e. qui ciblent celui-ci) à l'ensemble à mettre à jour.
    void updateRefresh(State& state);

    // Cherche des collisions avec des mobiles.
    virtual void updateCollisionsMobiles(State& state) = 0;

    // Teste la collision avec l'autre mobile.
    void testeCollision(Mobile* mobile, State& state);
    void testeCollision(const Collision& collision, State& state);

    // Paramètres.
    Coord<double> mPosition;
    Coord<double> mVitesse;
    double mMasse;

    QColor mColor;

private:
    // Supprime le mobile de la liste des événements.
    void detach(State& state);
    // Ajoute un mobile à la liste des cibles.
    bool addTarget(Mobile* mobile, State& state, bool addCollision = true);

    // Prochaine(s) collision(s).
    Time mTargetTime;
    std::set<std::multimap<Time, std::shared_ptr<Event> >::iterator> mNextCollisions;
    // Graphe des cibles.
    std::set<Mobile*> mTargets;
    std::set<Mobile*> mAttached;
    // Dernière(s) collision(s).
    Time mLastTime;
    std::list<std::shared_ptr<Collision> > mLastCollisions;

    // Identifiant unique du mobile.
    unsigned int mIndex;
    static unsigned int index;
};

// Accesseurs.
inline const Coord<double>& Mobile::position() const
    {return mPosition;}
inline const Coord<double>& Mobile::vitesse() const
    {return mVitesse;}
inline double Mobile::masse() const
    {return mMasse;}
inline QColor Mobile::color() const
    {return mColor;}
inline unsigned int Mobile::id() const
    {return mIndex;}

#endif // MOBILE_HPP
