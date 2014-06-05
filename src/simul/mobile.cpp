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

#include "mobile.hpp"

#include "coord_io.tpl"
#include "state.hpp"

// Identifiant unique du dernier mobile.
unsigned int Mobile::index = -1;

// Affichage dans un flux standard.
std::ostream& operator<<(std::ostream& flux, const Mobile& mobile)
{
    flux << "(id = " << mobile.mIndex << " ; pos = " << mobile.mPosition << " ; speed = " << mobile.mVitesse << ")";
    return flux;
}

// Comparaison pour utiliser un std::set.
bool operator<(const std::multimap<Time, std::shared_ptr<Event> >::iterator& it1, const std::multimap<Time, std::shared_ptr<Event> >::iterator& it2)
{
    return it1->second.get() < it2->second.get();
}


// Constructeur.
Mobile::Mobile(const Coord<double>& position, const Coord<double>& vitesse, const QColor& color, double masse) :
    mPosition(position),
    mVitesse(vitesse),
    mMasse(masse),
    mColor(color),
    mTargetTime(),
    mNextCollisions(),
    mTargets(),
    mAttached(),
    mIndex(++Mobile::index)
{
}


// Affiche la liste des cibles dans la sortie standard.
void Mobile::showTargets() const
{
    std::cout << "mTargets[" << *this << "].size() = " << mTargets.size() << std::endl;
    for (auto& target : mTargets)
        std::cout << "\ttarget : " << *target << std::endl;
}


// Avance le mobile jusqu'à l'instant indiqué (sans tenir compte des autres mobiles).
void Mobile::avance(const Time& time, const Coord<double>& gravity)
{
    mPosition += mVitesse * time.time() + gravity * time.time() * time.time() / 2;
    mVitesse += gravity * time.time();
}


// Calcule l'instant de la prochaine collision avec le mobile.
Time Mobile::collision(const Coord<double>&/* sommet*/, const Coord<double>&/* gravity*/) const
{
    return Time();
}

Time Mobile::collision(const Segment&/* segment*/, const Coord<double>&/* gravity*/) const
{
    return Time();
}

Time Mobile::newArea(double/* sizeArea*/, const Coord<double>&/* gravity*/) const
{
    return Time();
}


// Effectue la collision avec le mobile : calcul du changement de trajectoire et mise à jour des prochaines collisions.
void Mobile::doCollision(const Coord<double>&/* sommet*/, State& state)
{
    this->updateRefresh(state);
}

void Mobile::doCollision(const Segment&/* segment*/, State& state)
{
    this->updateRefresh(state);
}

// Effectue un changement de zone.
void Mobile::changeArea(State& state)
{
    this->updateRefresh(state);
}


// Définit la dernière collision du mobile.
void Mobile::setLastCollision(const Collision& collision, const Time& now)
{
    if (mLastTime != now)
        mLastCollisions.clear();
    mLastCollisions.push_back(std::make_shared<Collision>(collision));
    mLastTime = now;
}

// Vérifie que la collision est compatible avec la dernière collision du mobile.
bool Mobile::checkLastCollision(const Collision& collision, const Time& time) const
{
    for (auto& lastCollision : mLastCollisions)
        if (*lastCollision == collision && mLastTime == time)
            return false;
    return true;
}


// Cherche la prochaine collision de ce mobile et met à jour la table des événements.
void Mobile::updateCollisions(State& state)
{
    mTargetTime = Time();
    this->detach(state);

    // Recherche des collisions avec des mobiles.
    this->updateCollisionsMobiles(state);
}

// Détache le mobile de la collision (appelé en cas de changement de trajectoire).
void Mobile::detach(const Collision& collision)
{
    for (auto it = mNextCollisions.begin() ; it != mNextCollisions.end() ; ++it)
    {
        if (*std::dynamic_pointer_cast<Collision>((*it)->second) == collision)
        {
            mNextCollisions.erase(it);
            break;
        }
    }
}


// Ajoute tous les mobiles attachés (i.e. qui ciblent celui-ci) à l'ensemble à mettre à jour.
void Mobile::updateRefresh(State& state)
{
    state.toRefresh.insert(this);
    for (auto& attached : mAttached)
        state.toRefresh.insert(attached);
}


// Teste la collision avec l'autre mobile.
void Mobile::testeCollision(Mobile* mobile, State& state)
{
    Collision collision(this, mobile);
    Time time = collision.time(state);

    if (time < state.now || time.isNever())
        return;

    // La collision a lieu avant celle prévue précédemment.
    if (time < mTargetTime)
    {
        mTargetTime = time;
        this->detach(state);
    }
    if (time < mobile->mTargetTime)
    {
        mobile->mTargetTime = time;
        mobile->detach(state);
    }

    bool add = true;
    if (time == mTargetTime)
        add = !this->addTarget(mobile, state);
    if (time == mobile->mTargetTime)
        mobile->addTarget(this, state, add);
}

// Teste la collision.
void Mobile::testeCollision(const Collision& collision, State& state)
{
    Time time = collision.time(state);
    if (time < state.now || time.isNever())
        return;

    // La collision a lieu avant celle prévue précédemment.
    if (time < mTargetTime)
    {
        mTargetTime = time;
        this->detach(state);
    }

    if (time == mTargetTime)
        mNextCollisions.insert(
                    state.events.insert(
                        std::make_pair(
                            mTargetTime,
                            std::make_shared<Collision>(collision)
                        )));
}


// Supprime le mobile de la liste des événements.
void Mobile::detach(State& state)
{
    // Supprime les prochaines collisions (cibles) de la liste des événements et des mobiles concernés.
    for (auto it = mNextCollisions.begin() ; it != mNextCollisions.end() ; ++it)
    {
        std::shared_ptr<Event> copy = (*it)->second;
        state.events.erase(*it);
        std::dynamic_pointer_cast<Collision>(copy)->detach(this);
    }
    mNextCollisions.clear();

    // Détache ce mobile de ses cibles.
    for (auto& target : mTargets)
        target->mAttached.erase(this);
    mTargets.clear();
}

// Ajoute un mobile à la liste des cibles.
bool Mobile::addTarget(Mobile* mobile, State& state, bool addCollision)
{
    // Crée le double lien.
    mTargets.insert(mobile);
    mobile->mAttached.insert(this);

    // Ajoute la collision à la liste des événements si nécessaire.
    if (addCollision && mobile->mTargets.find(this) != mobile->mTargets.end())
    {
        auto it = state.events.insert(
                    std::make_pair(
                        mTargetTime,
                        std::make_shared<Collision>(this, mobile)
                    ));
        mNextCollisions.insert(it);
        mobile->mNextCollisions.insert(it);
        return true;
    }

    return false;
}
