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

#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <iostream>

#include "mobile.hpp"
#include "time.hpp"
#include "event.hpp"

// Evénement de collision d'un mobile avec une autre mobile, un obstacle ou un changement de zone.
class Collision : public Event
{
public:
    // Affichage dans un flux standard.
    friend std::ostream& operator<<(std::ostream& flux, const Collision& collision);

    // Effectue l'événement sur le simulateur en paramètre.
    bool perform(Simulateur& simulateur, bool& isDraw);

    // Constructeurs.
    Collision();
    Collision(Mobile* mobile1, Mobile* mobile2);
    Collision(Mobile* mobile, const Coord<double>& sommet);
    Collision(Mobile* mobile, const Segment& segment);
    Collision(Mobile* mobile); // Changement de zone.

    // Détache les deux mobiles (appelé si l'un d'eux change de trajectoire).
    void detach(const Mobile* mobile) const;

    // Calcule l'instant de cette collision.
    Time time(State& state) const;

    // Comparaison.
    bool operator==(const Collision& collision) const;
    inline bool operator!=(const Collision& collision) const;
    // Indique si une réelle collision à lieu.
    inline bool isReal() const;

    // Effectue la collision : calcul du changement de trajectoire et mise à jour des prochaines collisions.
    void doCollision(State& state);

private:
    enum Type {_defaut, _mobiles, _sommet, _segment, _area};

    // Eléments entrant en collision.
    Mobile* mMobile1;
    Mobile* mMobile2;
    Coord<double> mSommet;
    Segment mSegment;
    // Type de collision.
    Type mType;
};

// Comparaison.
inline bool Collision::operator!=(const Collision& collision) const
    {return !(*this == collision);}
// Indique si une réelle collision à lieu.
inline bool Collision::isReal() const
    {return mType != _area;}

#endif // COLLISION_HPP
