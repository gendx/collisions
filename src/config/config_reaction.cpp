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

#include "config_reaction.hpp"

// Ecriture d'un fichier de configuration.
QDataStream& operator<<(QDataStream& stream, const ConfigReaction& config)
{
    return stream << config.mPop1 << config.mPop2 << config.mType << config.mSeuil << config.m_Pop1 << config.m_Pop2;
}

// Lecture d'un fichier de configuration.
QDataStream& operator>>(QDataStream& stream, ConfigReaction& config)
{
    return stream >> config.mPop1 >> config.mPop2 >> config.mType >> config.mSeuil >> config.m_Pop1 >> config.m_Pop2;
}
