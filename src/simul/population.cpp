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

#include "population.hpp"

#include <iostream>
#include <limits>
#include "coord_io.tpl"
#include "piston.hpp"

void Population::create(const Configuration& config, QList<Population>& populations, unsigned int index, QMap<int, MapLigne>& mapMobiles, QList<std::shared_ptr<Piston> >& pistons, std::multimap<Time, std::shared_ptr<Event> >& events, const Time& now, double sizeArea)
{
    std::uniform_real_distribution<> distribX(mConfig.mPolygone.left(), mConfig.mPolygone.right());
    std::uniform_real_distribution<> distribY(mConfig.mPolygone.top(), mConfig.mPolygone.bottom());
    std::normal_distribution<> distribVitesse(0, mConfig.mVitesse);

    for (unsigned int i = 0 ; i < mConfig.mTaille ; ++i)
    {
        Coord<double> pos;

        do
            pos = Coord<double>(distribX(Solveur::generateur), distribY(Solveur::generateur));
        while (this->invalid(pos, config, populations, pistons));

        std::list<std::shared_ptr<Boule> >::iterator it = mBoules.insert(mBoules.end(), std::shared_ptr<Boule>(new Boule(pos, Coord<double>(distribVitesse(Solveur::generateur), distribVitesse(Solveur::generateur)), mConfig.mColor, mConfig.mMasse, mConfig.mRayon, now, sizeArea, mapMobiles)));
        mBoules.back()->setPopulation(now, index, it, events, config.configMutations());
        std::pair<unsigned int, unsigned int> countEtudes;
        mBoules.back()->updateCollisions(events, mapMobiles, now, sizeArea, config.gravity(), countEtudes);
    }
}

bool Population::invalid(const Coord<double>& pos, const Configuration& config, QList<Population>& populations, QList<std::shared_ptr<Piston> >& pistons)
{
    if ((mConfig.mPolygone.intersect(pos, mConfig.mRayon) || !mConfig.mPolygone.inside(pos))
        || (config.contour().sommets().intersect(pos, mConfig.mRayon) || !config.contour().sommets().inside(pos)))
        return true;

    for (int j = 0 ; j < config.obstacles().size() ; ++j)
        if (config.obstacles()[j].sommets().intersect(pos, mConfig.mRayon) || config.obstacles()[j].sommets().inside(pos))
            return true;

    for (int j = 0 ; j < pistons.size() ; ++j)
        if ((pistons[j]->position().y - pos.y) <= mConfig.mRayon && (pos.y - pistons[j]->position().y) <= mConfig.mRayon + pistons[j]->epaisseur())
            return true;

    for (std::list<std::shared_ptr<Boule> >::iterator it = mBoules.begin() ; it != mBoules.end() ; ++it)
        if (((*it)->position() - pos).length() <= 2 * mConfig.mRayon)
            return true;

    for (int j = 0 ; j < populations.size() ; ++j)
        for (std::list<std::shared_ptr<Boule> >::iterator it = populations[j].mBoules.begin() ; it != populations[j].mBoules.end() ; ++it)
            if (((*it)->position() - pos).length() <= mConfig.mRayon + populations[j].mConfig.mRayon)
                return true;

    return false;
}

double Population::meanFreeRide() const
{
    double result = 0;
    unsigned int nbre = 0;
    for (std::list<std::shared_ptr<Boule> >::const_iterator it = mBoules.begin() ; it != mBoules.end() ; ++it)
    {
        if ((*it)->validFree())
        {
            result += (*it)->freeRide().length();
            ++nbre;
        }
    }

    if (nbre > mBoules.size() * .9)
        return result / nbre;
    return std::numeric_limits<double>::quiet_NaN();
}

double Population::meanFreeTime() const
{
    double result = 0;
    unsigned int nbre = 0;
    for (std::list<std::shared_ptr<Boule> >::const_iterator it = mBoules.begin() ; it != mBoules.end() ; ++it)
    {
        if ((*it)->validFree())
        {
            result += (*it)->freeTime().time();
            ++nbre;
        }
    }

    if (nbre > mBoules.size() * .9)
        return result / nbre;
    return std::numeric_limits<double>::quiet_NaN();
}

double Population::meanFromOrigin() const
{
    double result = 0;
    for (std::list<std::shared_ptr<Boule> >::const_iterator it = mBoules.begin() ; it != mBoules.end() ; ++it)
        result += (*it)->fromOrigin().length();
    return result / mBoules.size();
}

double Population::squareFromOrigin() const
{
    double result = 0;
    for (std::list<std::shared_ptr<Boule> >::const_iterator it = mBoules.begin() ; it != mBoules.end() ; ++it)
        result += (*it)->fromOrigin().squareLength();
    return result / mBoules.size();
}

double Population::totalVitesse() const
{
    double result = 0;
    for (std::list<std::shared_ptr<Boule> >::const_iterator it = mBoules.begin() ; it != mBoules.end() ; ++it)
        result += (*it)->vitesse().length();
    return result;
}

double Population::totalVit2() const
{
    double result = 0;
    for (std::list<std::shared_ptr<Boule> >::const_iterator it = mBoules.begin() ; it != mBoules.end() ; ++it)
        result += (*it)->vitesse().squareLength();
    return result;
}
