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

#include "config_population.hpp"

// Ecriture d'un fichier de configuration.
QDataStream& operator<<(QDataStream& stream, const ConfigPopulation& config)
{
    return stream << config.mTaille << config.mRayon << config.mMasse << config.mColor << config.mPolygone << config.mVitesse;
}

// Lecture d'un fichier de configuration.
QDataStream& operator>>(QDataStream& stream, ConfigPopulation& config)
{
    return stream >> config.mTaille >> config.mRayon >> config.mMasse >> config.mColor >> config.mPolygone >> config.mVitesse;
}
