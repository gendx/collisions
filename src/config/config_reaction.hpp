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

#ifndef CONFIG_REACTION_HPP
#define CONFIG_REACTION_HPP

#include <QDataStream>

// Configuration d'une réaction.
// Une réaction est un changement de population dû à une collision entre boules.
class ConfigReaction
{
    friend class Boule;

public:
    enum Type
    {
        none = 0, proba = 1, energy = 2
    };

    // Constructeurs.
    inline ConfigReaction();
    inline ConfigReaction(unsigned int pop1, unsigned int pop2, Type type, double seuil, unsigned int _pop1, unsigned int _pop2);

    // Lecture/écriture pour les fichiers de configuration.
    friend QDataStream& operator<<(QDataStream& stream, const ConfigReaction& config);
    friend QDataStream& operator>>(QDataStream& stream, ConfigReaction& config);

    // Accesseurs.
    inline unsigned int pop1() const;
    inline unsigned int pop2() const;
    inline unsigned int _pop1() const;
    inline unsigned int _pop2() const;
    inline double seuil() const;
    inline unsigned int type() const;

private:
    // Populations sources.
    unsigned int mPop1;
    unsigned int mPop2;
    // Type de réaction.
    unsigned int mType;
    // Valeur seuil pour la réaction.
    double mSeuil;
    // Populations de destination.
    unsigned int m_Pop1;
    unsigned int m_Pop2;
};

// Constructeurs.
inline ConfigReaction::ConfigReaction() :
    mPop1(0), mPop2(0), mType(none), mSeuil(0), m_Pop1(0), m_Pop2(0) {}
inline ConfigReaction::ConfigReaction(unsigned int pop1, unsigned int pop2, Type type, double seuil, unsigned int _pop1, unsigned int _pop2) :
    mPop1(pop1), mPop2(pop2), mType(type), mSeuil(seuil), m_Pop1(_pop1), m_Pop2(_pop2) {}

// Accesseurs.
inline unsigned int ConfigReaction::pop1() const
    {return mPop1;}
inline unsigned int ConfigReaction::pop2() const
    {return mPop2;}
inline unsigned int ConfigReaction::_pop1() const
    {return m_Pop1;}
inline unsigned int ConfigReaction::_pop2() const
    {return m_Pop2;}
inline double ConfigReaction::seuil() const
    {return mSeuil;}
inline unsigned int ConfigReaction::type() const
    {return mType;}

#endif // CONFIG_REACTION_HPP
