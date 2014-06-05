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

#ifndef CONFIG_PISTON_HPP
#define CONFIG_PISTON_HPP

#include <QColor>

// Configuration d'un piston.
class ConfigPiston
{
public:
    // Constructeurs.
    inline ConfigPiston();
    inline ConfigPiston(double positionY, double vitesseY, const QColor& color, double masse, double epaisseur);

    // Lecture/écriture pour les fichiers de configuration.
    friend QDataStream& operator<<(QDataStream& stream, const ConfigPiston& config);
    friend QDataStream& operator>>(QDataStream& stream, ConfigPiston& config);

    // Paramètres du piston.
    double mPosition;
    double mVitesse;
    QColor mColor;
    double mMasse;
    double mEpaisseur;
};

// Constructeurs.
inline ConfigPiston::ConfigPiston() :
    mPosition(0), mVitesse(0), mColor(Qt::black), mMasse(0), mEpaisseur(0) {}
inline ConfigPiston::ConfigPiston(double positionY, double vitesseY, const QColor& color, double masse, double epaisseur) :
    mPosition(positionY), mVitesse(vitesseY), mColor(color), mMasse(masse), mEpaisseur(epaisseur) {}

#endif // CONFIG_PISTON_HPP
