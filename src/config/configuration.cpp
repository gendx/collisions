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

#include "configuration.hpp"

// Vérifie que la configuration est valide.
bool Configuration::check() const
{
    // Il faut au moins une population non vide.
    unsigned int nbre = 0;
    for (auto& pop : mConfigPops)
        nbre += pop.mTaille;
    if (!nbre)
        return false;

    // TODO : vérification selon la surface disponible.
    std::cout << "mContour.sommets().surface() = " << mContour.sommets().surface() << std::endl;

    return true;
}

// Taille adaptée au pavage.
double Configuration::sizeArea() const
{
    if (mConfigPops.empty())
        return 1;

    // Recherche du plus gros rayon d'une boule.
    double rayonMax = 0;
    for (auto& pop : mConfigPops)
    {
        double rayon = pop.mRayon;
        if (rayon > rayonMax)
            rayonMax = rayon;
    }

    // Taille > diamètre des boules.
    return rayonMax * 2.5;
}


// Ecriture d'un fichier de configuration.
QDataStream& operator<<(QDataStream& stream, const Configuration& config)
{
    return stream << config.mConfigPops << config.mConfigPistons << config.mObstacles << config.mContour << config.mConfigFcourbes << config.mConfigProfils << config.mGravity << config.mConfigReactions << config.mConfigMutations;
}

// Lecture d'un fichier de configuration.
QDataStream& operator>>(QDataStream& stream, Configuration& config)
{
    return stream >> config.mConfigPops >> config.mConfigPistons >> config.mObstacles >> config.mContour >> config.mConfigFcourbes >> config.mConfigProfils >> config.mGravity >> config.mConfigReactions >> config.mConfigMutations;
}


// Remise à zéro.
void Configuration::reset()
{
    mConfigPops.clear();
    mConfigPistons.clear();
    mObstacles.clear();
    mContour.reset();
    mConfigFcourbes.clear();
    mConfigProfils.clear();
    mConfigReactions.clear();
    mConfigMutations.clear();
}
