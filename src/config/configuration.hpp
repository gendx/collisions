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

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <QList>
#include "config_population.hpp"
#include "config_piston.hpp"
#include "obstacle.hpp"
#include "time.hpp"
#include "collision.hpp"
#include "config_widgetcourbe.hpp"
#include "config_profil.hpp"
#include "config_reaction.hpp"
#include "config_mutation.hpp"

// Configuration d'une simulation.
// Une configuration est associée à chaque document de l'interface graphique, et à chaque fichier ".col".
class Configuration
{
public:
    // Constructeurs.
    inline Configuration();
    inline Configuration(const QList<ConfigPopulation>& configPops, const QList<ConfigPiston>& configPistons, const QList<Obstacle>& obstacles, const Obstacle& contour, const QList<ConfigWidgetCourbe>& configFcourbes, const QList<ConfigProfil>& configProfils, const QList<ConfigReaction>& configReactions, const QList<ConfigMutation>& configMutations, const Coord<double>& gravity);

    // Vérifie que la configuration est valide.
    bool check() const;
    // Taille adaptée au pavage.
    // Ce pavage permet l'optimisation de la recherche de collision, mais doit être plus grand que le diamètre des boules, afin de ne pas rater de collision.
    double sizeArea() const;

    // Lecture/écriture pour les fichiers de configuration.
    friend QDataStream& operator<<(QDataStream& stream, const Configuration& config);
    friend QDataStream& operator>>(QDataStream& stream, Configuration& config);

    // Remise à zéro.
    void reset();

    // Accesseurs.
    inline QList<ConfigPopulation>& configPops();
    inline QList<ConfigPiston>& configPistons();
    inline QList<ConfigWidgetCourbe>& configFcourbes();
    inline QList<ConfigProfil>& configProfils();
    inline void setObstacles(const QList<Obstacle>& obstacles);
    inline void setContour(const Obstacle& contour);
    inline QList<ConfigReaction>& configReactions();
    inline QList<ConfigMutation>& configMutations();
    inline void setGravity(const Coord<double>& gravity);

    inline QList<ConfigPopulation> configPops() const;
    inline QList<ConfigPiston> configPistons() const;
    inline QList<ConfigWidgetCourbe> configFcourbes() const;
    inline QList<ConfigProfil> configProfils() const;
    inline const QList<Obstacle>& obstacles() const;
    inline const Obstacle& contour() const;
    inline const QList<ConfigReaction>& configReactions() const;
    inline const QList<ConfigMutation>& configMutations() const;
    inline const Coord<double>& gravity() const;

private:
    // Les mobiles.
    QList<ConfigPopulation> mConfigPops;
    QList<ConfigPiston> mConfigPistons;
    // Les obstacles.
    QList<Obstacle> mObstacles;
    Obstacle mContour;
    // Les courbes et profils.
    QList<ConfigWidgetCourbe> mConfigFcourbes;
    QList<ConfigProfil> mConfigProfils;
    // Les transformations.
    QList<ConfigReaction> mConfigReactions;
    QList<ConfigMutation> mConfigMutations;
    // Le vecteur de gravité.
    Coord<double> mGravity;
};


// Constructeurs.
inline Configuration::Configuration() :
    mConfigPops(), mConfigPistons(), mObstacles(), mContour(), mConfigFcourbes(), mConfigProfils(), mConfigReactions(), mConfigMutations(), mGravity() {}
inline Configuration::Configuration(const QList<ConfigPopulation>& configPops, const QList<ConfigPiston>& configPistons, const QList<Obstacle>& obstacles, const Obstacle& contour, const QList<ConfigWidgetCourbe>& configFcourbes, const QList<ConfigProfil>& configProfils, const QList<ConfigReaction>& configReactions, const QList<ConfigMutation>& configMutations, const Coord<double>& gravity) :
    mConfigPops(configPops), mConfigPistons(configPistons), mObstacles(obstacles), mContour(contour), mConfigFcourbes(configFcourbes), mConfigProfils(configProfils), mConfigReactions(configReactions), mConfigMutations(configMutations), mGravity(gravity) {}

// Accesseurs.
inline QList<ConfigPopulation>& Configuration::configPops()
    {return mConfigPops;}
inline QList<ConfigPiston>& Configuration::configPistons()
    {return mConfigPistons;}
inline QList<ConfigWidgetCourbe>& Configuration::configFcourbes()
    {return mConfigFcourbes;}
inline QList<ConfigProfil>& Configuration::configProfils()
    {return mConfigProfils;}
inline void Configuration::setObstacles(const QList<Obstacle>& obstacles)
    {mObstacles = obstacles;}
inline void Configuration::setContour(const Obstacle& contour)
    {mContour = contour;}
inline QList<ConfigReaction>& Configuration::configReactions()
    {return mConfigReactions;}
inline QList<ConfigMutation>& Configuration::configMutations()
    {return mConfigMutations;}
inline void Configuration::setGravity(const Coord<double>& gravity)
    {mGravity = gravity;}

inline QList<ConfigPopulation> Configuration::configPops() const
    {return mConfigPops;}
inline QList<ConfigPiston> Configuration::configPistons() const
    {return mConfigPistons;}
inline QList<ConfigWidgetCourbe> Configuration::configFcourbes() const
    {return mConfigFcourbes;}
inline QList<ConfigProfil> Configuration::configProfils() const
    {return mConfigProfils;}
inline const QList<Obstacle>& Configuration::obstacles() const
    {return mObstacles;}
inline const Obstacle& Configuration::contour() const
    {return mContour;}
inline const QList<ConfigReaction>& Configuration::configReactions() const
    {return mConfigReactions;}
inline const QList<ConfigMutation>& Configuration::configMutations() const
    {return mConfigMutations;}
inline const Coord<double>& Configuration::gravity() const
    {return mGravity;}

#endif // CONFIGURATION_HPP
