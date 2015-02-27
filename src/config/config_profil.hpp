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

#ifndef CONFIG_PROFIL_HPP
#define CONFIG_PROFIL_HPP

#include "config_cible.hpp"

// Configuration d'un profil.
// Un profil est un affichage d'une grandeur selon des tranches de l'espace.
class ConfigProfil
{
public:
    enum Type
    {
        none = 0, energy = 1, count = 2
    };

    // Constructeurs.
    inline ConfigProfil();
    inline ConfigProfil(Type type, bool mean, double slice, const QList<ConfigCible>& cibles, const Polygone& polygone);

    // Lecture/écriture pour les fichiers de configuration.
    friend QDataStream& operator<<(QDataStream& stream, const ConfigProfil& config);
    friend QDataStream& operator>>(QDataStream& stream, ConfigProfil& config);

    // Type de grandeur.
    unsigned int mType;
    // Calcul de valeur moyenne ou non.
    bool mMean;
    // Largeur d'une tranche.
    double mSlice;
    // Cibles concernées.
    QList<ConfigCible> mCibles;
    // Zone de l'espace concernée.
    Polygone mPolygone;
};

// Constructeurs.
inline ConfigProfil::ConfigProfil() :
    mType(none),mMean(false), mSlice(0), mCibles(), mPolygone() {}
inline ConfigProfil::ConfigProfil(Type type, bool mean, double slice, const QList<ConfigCible>& cibles, const Polygone& polygone) :
    mType(type), mMean(mean), mSlice(slice), mCibles(cibles), mPolygone(polygone) {}

#endif // CONFIG_FPROFIL_HPP
