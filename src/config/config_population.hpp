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

#ifndef CONFIG_POPULATION_HPP
#define CONFIG_POPULATION_HPP

#include <QColor>
#include "polygone.hpp"

// Configuration d'une population.
// Une population est un ensemble de boules.
// La configuration indique la position initiale de la population, le nombre de boules, etc...
class ConfigPopulation
{
    friend class Population;

public:
    // Constructeurs.
    inline ConfigPopulation();
    inline ConfigPopulation(unsigned int taille, double rayon, double masse, const QColor& color, const Polygone& polygone, double vitesse);

    // Lecture/écriture pour les fichiers de configuration.
    friend QDataStream& operator<<(QDataStream& stream, const ConfigPopulation& config);
    friend QDataStream& operator>>(QDataStream& stream, ConfigPopulation& config);

    // Accesseurs.
    inline unsigned int taille() const;
    inline double rayon() const;
    inline double masse() const;
    inline QColor color() const;
    inline Polygone polygone() const;
    inline double vitesse() const;

private:
    // Paramètres de la population.
    // Nombre de boules.
    unsigned int mTaille;
    // Paramètres des boules
    double mRayon;
    double mMasse;
    QColor mColor;
    // Zone initiale.
    Polygone mPolygone;
    // Vitesse initiale (paramètre d'une loi normale).
    double mVitesse;
};

// Constructeurs.
inline ConfigPopulation::ConfigPopulation() :
    mTaille(0), mRayon(0), mMasse(0), mColor(Qt::black) {}
inline ConfigPopulation::ConfigPopulation(unsigned int taille, double rayon, double masse, const QColor& color, const Polygone& polygone, double vitesse) :
    mTaille(taille), mRayon(rayon), mMasse(masse), mColor(color), mPolygone(polygone), mVitesse(vitesse) {}

// Accesseurs.
inline unsigned int ConfigPopulation::taille() const
    {return mTaille;}
inline double ConfigPopulation::rayon() const
    {return mRayon;}
inline double ConfigPopulation::masse() const
    {return mMasse;}
inline QColor ConfigPopulation::color() const
    {return mColor;}
inline Polygone ConfigPopulation::polygone() const
    {return mPolygone;}
inline double ConfigPopulation::vitesse() const
    {return mVitesse;}

#endif // CONFIG_POPULATION_HPP
