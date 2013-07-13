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

#include "mobile.hpp"

#include "coord_io.tpl"
#include "population.hpp"
#include "configuration.hpp"
#include "piston.hpp"

// Identifiant unique du dernier mobile.
unsigned int Mobile::index = -1;

// Affichage dans un flux standard.
std::ostream& operator<<(std::ostream& flux, const Mobile& mobile)
{
    flux << "(id = " << mobile.mIndex << " ; pos = " << mobile.mPosition << " ; speed = " << mobile.mVitesse << ")";
    return flux;
}

// Comparaison pour utiliser un std::set.
bool operator<(const std::multimap<Time, boost::shared_ptr<Event> >::iterator& it1, const std::multimap<Time, boost::shared_ptr<Event> >::iterator& it2)
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
    for (std::set<Mobile*>::iterator it = mTargets.begin() ; it != mTargets.end() ; ++it)
        std::cout << "\ttarget : " << **it << std::endl;
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
void Mobile::doCollision(const Time&/* now*/, const Coord<double>&/* sommet*/, std::set<Mobile*>& toRefresh, std::pair<unsigned int, unsigned int>& countEtudes)
{
    this->updateRefresh(toRefresh);
}

void Mobile::doCollision(const Time&/* now*/, const Segment&/* segment*/, std::set<Mobile*>& toRefresh, std::pair<unsigned int, unsigned int>& countEtudes)
{
    this->updateRefresh(toRefresh);
}

// Effectue un changement de zone.
void Mobile::changeArea(double/* sizeArea*/, std::set<Mobile*>& toRefresh, QMap<int, MapLigne>&/* mapMobiles*/, std::pair<unsigned int, unsigned int>& countEtudes)
{
    this->updateRefresh(toRefresh);
}


// Définit la dernière collision du mobile.
void Mobile::setLastCollision(const Time& now, const Collision& collision)
{
    if (mLastTime != now)
        mLastCollisions.clear();
    mLastCollisions.append(boost::shared_ptr<Collision>(new Collision(collision)));
    mLastTime = now;
}

// Vérifie que la collision est compatible avec la dernière collision du mobile.
bool Mobile::checkLastCollision(const Collision& collision, const Time& time) const
{
    /*
    std::cout.precision(15);
    std::cout << "mLastTime = " << mLastTime << " ; time = " << time << std::endl;
    //*/
    /*
    return (mLastTime != time);
    //return true;
    /*/
    for (int i = 0 ; i < mLastCollisions.size() ; ++i)
        if (*mLastCollisions[i] == collision && mLastTime == time)
            return false;
    return true;
    //*/
}


// Cherche la prochaine collision de ce mobile et met à jour la table des événements.
void Mobile::updateCollisions(std::multimap<Time, boost::shared_ptr<Event> >& events, QMap<int, MapLigne>& mapMobiles, const Time& now, double sizeArea, const Coord<double>& gravity, std::pair<unsigned int, unsigned int>& countEtudes)
{
    mTargetTime = Time();
    this->detach(events);

    // Recherche des collisions avec des mobiles.
    this->updateCollisionsMobiles(events, mapMobiles, now, sizeArea, gravity, countEtudes);
}

// Détache le mobile de la collision (appelé en cas de changement de trajectoire).
void Mobile::detach(const Collision& collision)
{
    for (std::set<std::multimap<Time, boost::shared_ptr<Event> >::iterator>::iterator it = mNextCollisions.begin() ; it != mNextCollisions.end() ; ++it)
    {
        if (*reinterpret_cast<Collision*>((*it)->second.get()) == collision)
        {
            mNextCollisions.erase(it);
            break;
        }
    }
}


// Ajoute tous les mobiles attachés (i.e. qui ciblent celui-ci) à l'ensemble à mettre à jour.
void Mobile::updateRefresh(std::set<Mobile*>& toRefresh)
{
    toRefresh.insert(this);
    for (std::set<Mobile*>::iterator it = mAttached.begin() ; it != mAttached.end() ; ++it)
        toRefresh.insert(*it);
}


// Teste la collision avec l'autre mobile.
void Mobile::testeCollision(Mobile* mobile, std::multimap<Time, boost::shared_ptr<Event> >& events, const Time& now, double sizeArea, const Coord<double>& gravity, std::pair<unsigned int, unsigned int>& countEtudes)
{
    Collision collision(this, mobile);
    Time time = collision.time(now, sizeArea, gravity, countEtudes);

    if (time < now || time.isNever())
        return;

    // La collision a lieu avant celle prévue précédemment.
    if (time < mTargetTime)
    {
        mTargetTime = time;
        this->detach(events);
    }
    if (time < mobile->mTargetTime)
    {
        mobile->mTargetTime = time;
        mobile->detach(events);
    }

    bool add = true;
    if (time == mTargetTime)
        add = !this->addTarget(mobile, events);
    if (time == mobile->mTargetTime)
        mobile->addTarget(this, events, add);
}

// Teste la collision.
void Mobile::testeCollision(const Collision& collision, std::multimap<Time, boost::shared_ptr<Event> >& events, const Time& now, double sizeArea, const Coord<double>& gravity, std::pair<unsigned int, unsigned int>& countEtudes)
{
    Time time = collision.time(now, sizeArea, gravity, countEtudes);
    if (time < now || time.isNever())
        return;

    // La collision a lieu avant celle prévue précédemment.
    if (time < mTargetTime)
    {
        mTargetTime = time;
        this->detach(events);
    }
    if (time == mTargetTime)
        mNextCollisions.insert(events.insert(std::make_pair(mTargetTime, boost::shared_ptr<Event>(new Collision(collision)))));
}


// Supprime le mobile de la liste des événements.
void Mobile::detach(std::multimap<Time, boost::shared_ptr<Event> >& events)
{
    // Supprime les prochaines collisions (cibles) de la liste des événements et des mobiles concernés.
    for (std::set<std::multimap<Time, boost::shared_ptr<Event> >::iterator>::iterator it = mNextCollisions.begin() ; it != mNextCollisions.end() ; ++it)
    {
        boost::shared_ptr<Event> copy = (*it)->second;
        events.erase(*it);
        reinterpret_cast<Collision*>(copy.get())->detach(this);
    }
    mNextCollisions.clear();

    // Détache ce mobile de ses cibles.
    for (std::set<Mobile*>::iterator it = mTargets.begin() ; it != mTargets.end() ; ++it)
        (*it)->mAttached.erase(this);
    mTargets.clear();
}

// Ajoute un mobile à la liste des cibles.
bool Mobile::addTarget(Mobile* mobile, std::multimap<Time, boost::shared_ptr<Event> >& events, bool addCollision)
{
    // Crée le double lien.
    mTargets.insert(mobile);
    mobile->mAttached.insert(this);

    // Ajoute la collision à la liste des événements si nécessaire.
    if (addCollision && mobile->mTargets.find(this) != mobile->mTargets.end())
    {
        std::multimap<Time, boost::shared_ptr<Event> >::iterator it = events.insert(std::make_pair(mTargetTime, boost::shared_ptr<Event>(new Collision(this, mobile))));
        mNextCollisions.insert(it);
        mobile->mNextCollisions.insert(it);
        return true;
    }

    return false;
}
