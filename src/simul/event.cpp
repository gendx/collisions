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

#include "event.hpp"

#include "simulateur.hpp"

// Constructeur.
BouleEvent::BouleEvent(Boule* boule) :
    mBoule(boule)
{
}


// Ajoute l'événement au simulateur en paramètre.
void Event::addEvent(Simulateur&/* simulateur*/)
{
}

void DrawEvent::addEvent(Simulateur& simulateur)
{
    simulateur.addDrawEvent();
}

void ValueEvent::addEvent(Simulateur& simulateur)
{
    simulateur.addValueEvent();
}

void CourbeEvent::addEvent(Simulateur& simulateur)
{
    simulateur.addCourbeEvent();
}


// Effectue l'événement sur le simulateur en paramètre.
bool DrawEvent::perform(Simulateur& simulateur, bool& isDraw)
{
    isDraw = true;
    return simulateur.performDrawEvent();
}

bool ValueEvent::perform(Simulateur& simulateur, bool&/* isDraw*/)
{
    return simulateur.performValueEvent();
}

bool CourbeEvent::perform(Simulateur& simulateur, bool&/* isDraw*/)
{
    return simulateur.performCourbeEvent();
}

bool BouleEvent::perform(Simulateur& simulateur, bool&/* isDraw*/)
{
    return simulateur.performBouleEvent(mBoule);
}
