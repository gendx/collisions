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

#ifndef CONFIG_COURBE_HPP
#define CONFIG_COURBE_HPP

#include <QColor>
#include "polygone.hpp"
#include "config_cible.hpp"

// Configuration d'une courbe.
class ConfigCourbe
{
    friend class Courbe;

public:
    // Constructeurs.
    inline ConfigCourbe();
    inline ConfigCourbe(const QList<ConfigCible>& cibles, const QColor& color, const Polygone& polygone);

    // Lecture/écriture pour les fichiers de configuration.
    friend QDataStream& operator<<(QDataStream& stream, const ConfigCourbe& config);
    friend QDataStream& operator>>(QDataStream& stream, ConfigCourbe& config);

private:
    // Ensemble de cibles concernées.
    QList<ConfigCible> mCibles;
    // Couleur de la courbe.
    QColor mColor;
    // Zone de l'espace concernée.
    Polygone mPolygone;
};

// Constructeurs.
inline ConfigCourbe::ConfigCourbe() :
    mCibles(), mColor(Qt::black), mPolygone() {}
inline ConfigCourbe::ConfigCourbe(const QList<ConfigCible>& cibles, const QColor& color, const Polygone& polygone) :
    mCibles(cibles), mColor(color), mPolygone(polygone) {}

#endif // CONFIG_COURBE_HPP
