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

#include "config_cible.hpp"

#include <limits>

#include "config_widgetcourbe.hpp"
#include "config_profil.hpp"
#include "population.hpp"
#include "piston.hpp"

QDataStream& operator<<(QDataStream& stream, const ConfigCible& config)
{
    return stream << config.mType << config.mIndex << config.mPolygone;
}

QDataStream& operator>>(QDataStream& stream, ConfigCible& config)
{
    return stream >> config.mType >> config.mIndex >> config.mPolygone;
}


void ConfigCible::addValue(unsigned int valType, const QList<Population>& populations, const Polygone& polygone, QMap<int, double>& values, QMap<int, unsigned int>& nbres, double slice)
{
    if (mType != _population || mIndex >= populations.size())
        return;

    for (std::list<std::shared_ptr<Boule> >::const_iterator it = populations[mIndex].boules().begin() ; it != populations[mIndex].boules().end() ; ++it)
    {
        const Boule& boule = **it;
        if (mPolygone.inside(boule.position()) && polygone.inside(boule.position()))
        {
            if (values.contains(floor(boule.position().y / slice)))
            {
                values[floor(boule.position().y / slice)] += this->profilValue(valType, boule);
                ++nbres[floor(boule.position().y / slice)];
            }
            else
            {
                values[floor(boule.position().y / slice)] = this->profilValue(valType, boule);
                nbres[floor(boule.position().y / slice)] = 1;
            }
        }
    }
}


double ConfigCible::value(unsigned int valType, const QList<Population>& populations, const Polygone& polygone, const QList<std::shared_ptr<Piston> >& pistons, unsigned int& nbre)
{
    if (mType == _piston)
    {
        if (mIndex < pistons.size())
        {
            ++nbre;
            return this->value(valType, *pistons[mIndex]);
        }
    }
    else if (mType == _population)
    {
        if (mIndex < populations.size())
            return this->value(valType, populations[mIndex], polygone, nbre);
    }

    return std::numeric_limits<double>::quiet_NaN();
}

double ConfigCible::value(unsigned int valType, const Piston& piston)
{
    if (valType == ConfigWidgetCourbe::posX)
        return piston.position().x;
    if (valType == ConfigWidgetCourbe::posY)
        return -piston.position().y;
    if (valType == ConfigWidgetCourbe::vitX)
        return piston.vitesse().x;
    if (valType == ConfigWidgetCourbe::vitY)
        return -piston.vitesse().y;
    if (valType == ConfigWidgetCourbe::vit)
        return piston.vitesse().length();
    if (valType == ConfigWidgetCourbe::vit2)
        return piston.vitesse().squareLength();
    if (valType == ConfigWidgetCourbe::energy)
        return piston.masse() * piston.vitesse().squareLength();
    if (valType == ConfigWidgetCourbe::count)
        return 1;

    return std::numeric_limits<double>::quiet_NaN();
}

double ConfigCible::value(unsigned int valType, const Population& population, const Polygone& polygone, unsigned int& nbre)
{
    if (valType == ConfigWidgetCourbe::none)
        return std::numeric_limits<double>::quiet_NaN();

    double valeur = 0;

    for (std::list<std::shared_ptr<Boule> >::const_iterator it = population.boules().begin() ; it != population.boules().end() ; ++it)
    //for (unsigned int i = 0 ; i < population.boules().size() ; ++i)
    {
        const Boule& boule = **it;
        if (mPolygone.inside(boule.position()) && polygone.inside(boule.position()))
        {
            valeur += this->value(valType, boule);
            ++nbre;
        }
    }

    return valeur;
}

double ConfigCible::value(unsigned int valType, const Boule& boule)
{
    if (valType == ConfigWidgetCourbe::posX)
        return boule.position().x;
    if (valType == ConfigWidgetCourbe::posY)
        return -boule.position().y;
    if (valType == ConfigWidgetCourbe::vitX)
        return boule.vitesse().x;
    if (valType == ConfigWidgetCourbe::vitY)
        return -boule.vitesse().y;
    if (valType == ConfigWidgetCourbe::vit)
        return boule.vitesse().length();
    if (valType == ConfigWidgetCourbe::vit2)
        return boule.vitesse().squareLength();
    if (valType == ConfigWidgetCourbe::energy)
        return boule.masse() * boule.vitesse().squareLength();
    if (valType == ConfigWidgetCourbe::freeRide)
        return boule.freeRide().length();
    if (valType == ConfigWidgetCourbe::freeTime)
        return boule.freeTime().time();
    if (valType == ConfigWidgetCourbe::fromOrigin)
        return boule.fromOrigin().length();
    if (valType == ConfigWidgetCourbe::fromOrigin2)
        return boule.fromOrigin().squareLength();
    if (valType == ConfigWidgetCourbe::count)
        return 1;

    return std::numeric_limits<double>::quiet_NaN();
}

double ConfigCible::profilValue(unsigned int valType, const Boule& boule)
{
    if (valType == ConfigProfil::energy)
        return boule.masse() * boule.vitesse().squareLength();
    if (valType == ConfigProfil::count)
        return 1;

    return std::numeric_limits<double>::quiet_NaN();
}
