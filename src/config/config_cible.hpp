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

#ifndef CONFIG_CIBLE_HPP
#define CONFIG_CIBLE_HPP

#include <QDataStream>
#include <memory>
#include "polygone.hpp"

class State;
class Population;
class Piston;
class Boule;

// Configuration d'une cible.
// Les cibles représentent un ensemble de mobiles pour lesquels tracer une courbe.
class ConfigCible
{
public:
    enum Type
    {
        _none = 0, _population = 1, _piston = 2
    };

    // Constructeurs.
    inline ConfigCible();
    inline ConfigCible(Type type, unsigned int index, const Polygone& polygone);

    // Calcule la valeur associée à l'ensemble concerné.
    double value(unsigned int valType, const Polygone& polygone, State& state, unsigned int& nbre);
    // Ajoute une tranche pour un profil.
    void addValue(unsigned int valType, const Polygone& polygone, State& state, QMap<int, double>& values, QMap<int, unsigned int>& nbres, double slice);

    // Lecture/écriture pour les fichiers de configuration.
    friend QDataStream& operator<<(QDataStream& stream, const ConfigCible& config);
    friend QDataStream& operator>>(QDataStream& stream, ConfigCible& config);

private:
    // Mesure une valeur sur un mobile.
    double value(unsigned int valType, const Piston& piston);
    double value(unsigned int valType, const Population& population, const Polygone& polygone, unsigned int& nbre);
    double value(unsigned int valType, const Boule& boule);
    double profilValue(unsigned int valType, const Boule& boule);

    // Ensemble représenté.
    unsigned int mType;
    int mIndex;
    // Zone de l'espace concernée.
    Polygone mPolygone;
};

// Constructeurs.
inline ConfigCible::ConfigCible() :
    mType(_none), mIndex(0), mPolygone() {}
inline ConfigCible::ConfigCible(Type type, unsigned int index, const Polygone& polygone) :
    mType(type), mIndex(index), mPolygone(polygone) {}

#endif // CONFIG_CIBLE_HPP
