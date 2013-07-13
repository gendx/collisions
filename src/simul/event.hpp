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

#ifndef EVENT_HPP
#define EVENT_HPP

class Simulateur;
class Boule;

// Classe abstraite qui décrit un événement de la simulation.
class Event
{
public:
    // Effectue l'événement sur le simulateur en paramètre.
    virtual bool perform(Simulateur& simulateur, bool& isDraw) = 0;
    // Ajoute l'événement au simulateur en paramètre.
    virtual void addEvent(Simulateur& simulateur);
};

// Evénement de dessin sur le widget.
class DrawEvent : public Event
{
public:
    virtual bool perform(Simulateur& simulateur, bool& isDraw);
    virtual void addEvent(Simulateur& simulateur);
};

// Evénement de récupération de valeurs pour les courbes.
class ValueEvent : public Event
{
public:
    virtual bool perform(Simulateur& simulateur, bool& isDraw);
    virtual void addEvent(Simulateur& simulateur);
};

// Evénement d'affichage des courbes.
class CourbeEvent : public Event
{
public:
    virtual bool perform(Simulateur& simulateur, bool& isDraw);
    virtual void addEvent(Simulateur& simulateur);
};

// Evénement de changement de population pour une particule.
class BouleEvent : public Event
{
public:
    BouleEvent(Boule* boule);

    virtual bool perform(Simulateur& simulateur, bool& isDraw);

private:
    Boule* mBoule;
};

#endif // EVENT_HPP
