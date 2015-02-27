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

#include "collision.hpp"

#include <cmath>
#include "coord_io.tpl"
#include "document.hpp"


// Affichage dans un flux standard.
std::ostream& operator<<(std::ostream& flux, const Collision& collision)
{
    flux << "{";
    if (collision.mType == Collision::_defaut)
        flux << "defaut";
    else if (collision.mType == Collision::_mobiles)
        flux << *collision.mMobile1 << " ; " << *collision.mMobile2;
    else if (collision.mType == Collision::_sommet)
        flux << *collision.mMobile1 << " : " << collision.mSommet;
    else if (collision.mType == Collision::_segment)
        flux << *collision.mMobile1 << " -> " << collision.mSegment.vect();
    else if (collision.mType == Collision::_area)
        flux << *collision.mMobile1 << " = area";
    flux << "}";
    return flux;
}


// Constructeurs.
Collision::Collision() :
    mType(_defaut)
{
}

Collision::Collision(Mobile* mobile1, Mobile* mobile2) :
    mMobile1(mobile1),
    mMobile2(mobile2),
    mType(_mobiles)
{
}

Collision::Collision(Mobile* mobile, const Coord<double>& sommet) :
    mMobile1(mobile),
    mSommet(sommet),
    mType(_sommet)
{
}

Collision::Collision(Mobile* mobile, const Segment& segment) :
    mMobile1(mobile),
    mSegment(segment),
    mType(_segment)
{
}

Collision::Collision(Mobile* mobile) :
    mMobile1(mobile),
    mType(_area)
{
}


// Effectue l'événement sur le simulateur en paramètre.
bool Collision::perform(Simulateur& simulateur, bool&/* isDraw*/)
{
    return simulateur.performCollision(*this);
}


// Détache les deux mobiles (appelé si l'un d'eux change de trajectoire).
void Collision::detach(const Mobile* mobile) const
{
    if (mType == _mobiles)
    {
        if (mMobile1 == mobile)
            mMobile2->detach(*this);
        else if (mMobile2 == mobile)
            mMobile1->detach(*this);
    }
}


// Calcule l'instant de cette collision.
Time Collision::time(State& state) const
{
    if (mType == _defaut)
        return Time();

    ++state.countEtudes.first;

    // Utilise les méthodes implémentées par les classes de mobiles.
    Time time;
    if (mType == _mobiles)
    {
        time = mMobile1->collision(mMobile2);
        ++state.countEtudes.second;
    }
    else if (mType == _sommet)
        time = mMobile1->collision(mSommet, state.config.gravity());
    else if (mType == _segment)
        time = mMobile1->collision(mSegment, state.config.gravity());
    else if (mType == _area)
        time = mMobile1->newArea(state.sizeArea, state.config.gravity());

    // Empêche d'effectuer la même collision deux fois de suite (à cause d'erreurs d'arrondis).
    // Cependant, la condition doit normalement être toujours fausse.
    // TODO : valider la suppression des lignes suivantes.
    if (mType != _area && (!mMobile1->checkLastCollision(*this, state.now + time)) && (mType != _mobiles || (!mMobile2->checkLastCollision(*this, state.now + time))))
    {
        std::cout << "rejected : " << *this << std::endl;
        return Time();
    }

    return state.now + time;
}

// Comparaison.
bool Collision::operator==(const Collision& collision) const
{
    if (mType != collision.mType)
        return false;

    else if (mType == _defaut)
        return true;
    else if (mType == _mobiles)
        return (mMobile2 == collision.mMobile2 && mMobile1 == collision.mMobile1) || (mMobile2 == collision.mMobile1 && mMobile1 == collision.mMobile2);
    else if (mType == _sommet)
        return mMobile1 == collision.mMobile1 && mSommet == collision.mSommet;
    else if (mType == _segment)
        return mMobile1 == collision.mMobile1 && mSegment == collision.mSegment;
    else if (mType == _area)
        return mMobile1 == collision.mMobile1;
    return false;
}


// Effectue la collision : calcul du changement de trajectoire et mise à jour des prochaines collisions.
void Collision::doCollision(State& state)
{
    mMobile1->setLastCollision(*this, state.now);

    if (mType == _defaut)
        return;
    else if (mType == _mobiles)
    {
        mMobile2->setLastCollision(*this, state.now);
        return mMobile1->doCollision(mMobile2, state);
    }
    else if (mType == _sommet)
        return mMobile1->doCollision(mSommet, state);
    else if (mType == _segment)
        return mMobile1->doCollision(mSegment, state);
    else if (mType == _area)
        return mMobile1->changeArea(state);
}
