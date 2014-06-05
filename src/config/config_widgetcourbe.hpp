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

#ifndef CONFIG_WIDGETCOURBE_HPP
#define CONFIG_WIDGETCOURBE_HPP

#include "config_courbe.hpp"

// Configuration d'un widget de courbe.
// Un widget de courbe est un ensemble de courbes représentant la même grandeur.
class ConfigWidgetCourbe
{
public:
    enum Type
    {
        none = 0, posX = 1, posY = 2, vitX = 3, vitY = 4, vit = 5, vit2 = 6, energy = 7, freeRide = 8, freeTime = 9, fromOrigin = 10, fromOrigin2 = 11, count = 12
    };

    // Constructeurs.
    inline ConfigWidgetCourbe();
    inline ConfigWidgetCourbe(Type type, bool mean, const QList<ConfigCourbe>& courbes);

    // Lecture/écriture pour les fichiers de configuration.
    friend QDataStream& operator<<(QDataStream& stream, const ConfigWidgetCourbe& config);
    friend QDataStream& operator>>(QDataStream& stream, ConfigWidgetCourbe& config);

    // Type de grandeur.
    unsigned int mType;
    // Calcul de valeur moyenne ou non.
    bool mMean;
    // Courbes à afficher.
    QList<ConfigCourbe> mCourbes;
};

// Constructeurs.
inline ConfigWidgetCourbe::ConfigWidgetCourbe() :
    mType(none), mMean(false), mCourbes() {}
inline ConfigWidgetCourbe::ConfigWidgetCourbe(Type type, bool mean, const QList<ConfigCourbe>& courbes) :
    mType(type), mMean(mean), mCourbes(courbes) {}

#endif // CONFIG_WIDGETCOURBE_HPP
