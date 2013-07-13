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

#ifndef CONFIG_MUTATION_HPP
#define CONFIG_MUTATION_HPP

#include <QDataStream>

// Configuration d'une mutation.
// Une mutation est un changement spontané de population pour une boule.
class ConfigMutation
{
    friend class Boule;

public:
    enum Type
    {
        time = 0, proba = 1
    };

    // Constructeurs.
    inline ConfigMutation();
    inline ConfigMutation(unsigned int pop1, unsigned int pop2, Type type, double tau);

    // Lecture/écriture pour les fichiers de configuration.
    friend QDataStream& operator<<(QDataStream& stream, const ConfigMutation& config);
    friend QDataStream& operator>>(QDataStream& stream, ConfigMutation& config);

    // Accesseurs.
    inline unsigned int pop1() const;
    inline unsigned int pop2() const;
    inline double tau() const;
    inline unsigned int type() const;

private:
    // Populations source et destination.
    unsigned int mPop1;
    unsigned int mPop2;
    // Type de mutation.
    unsigned int mType;
    // Valeur seuil de la mutation.
    double mTau;
};

// Constructeurs.
inline ConfigMutation::ConfigMutation() :
    mPop1(0), mPop2(0), mType(time), mTau(0) {}
inline ConfigMutation::ConfigMutation(unsigned int pop1, unsigned int pop2, Type type, double tau) :
    mPop1(pop1), mPop2(pop2), mType(type), mTau(tau) {}

// Accesseurs.
inline unsigned int ConfigMutation::pop1() const
    {return mPop1;}
inline unsigned int ConfigMutation::pop2() const
    {return mPop2;}
inline double ConfigMutation::tau() const
    {return mTau;}
inline unsigned int ConfigMutation::type() const
    {return mType;}

#endif // CONFIG_MUTATION_HPP
