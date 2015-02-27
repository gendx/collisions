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

#include "piston.hpp"

#include "state.hpp"
#include "solveur.hpp"

// Constructeur.
Piston::Piston(ConfigPiston config, State& state) :
    Mobile(Coord<double>(0, config.mPosition), Coord<double>(0, config.mVitesse), config.mColor, config.mMasse),
    mEpaisseur(config.mEpaisseur),
    mArea1(std::floor(mPosition.y / state.sizeArea)),
    mArea2(std::floor((mPosition.y + mEpaisseur) / state.sizeArea))
{
    // Mise à jour de la carte.
    auto& pistons1 = state.mapMobiles[mArea1].pistons();
    pistons1.prepend(this);
    mMapIt1 = pistons1.begin();

    auto& pistons2 = state.mapMobiles[mArea2].pistons();
    pistons2.prepend(this);
    mMapIt2 = pistons2.begin();
}


// Calcule l'instant de la prochaine collision avec le mobile.
Time Piston::collision(const Mobile* mobile) const
{
    return mobile->collision(this);
}

Time Piston::collision(const Boule* boule) const
{
    return boule->collision(this);
}

Time Piston::collision(const Piston* piston) const
{
    // Différence de vitesse.
    double dVitesseY = mVitesse.y - piston->mVitesse.y;
    double dPositionY = mPosition.y - piston->mPosition.y;

    // Différence de position.
    if (dPositionY >= 0)
        dPositionY -= piston->mEpaisseur;
    else
        dPositionY += mEpaisseur;

    // Calcul de l'instant.
    return Time(dPositionY / dVitesseY);
}

// Calcule l'instant du prochain changement de zone.
Time Piston::newArea(double sizeArea, const Coord<double>& gravity) const
{
    Time result;
    Time tmp;

    // Résolution selon le signe de la gravité.
    if (gravity.y == 0.0)
    {
        if (mVitesse.y > 0)
        {
            result = ((mArea1 + 1) * sizeArea - mPosition.y) / mVitesse.y;
            tmp = ((mArea2 + 1) * sizeArea - mPosition.y - mEpaisseur) / mVitesse.y;
        }
        else if (mVitesse.y < 0)
        {
            result = (mArea1 * sizeArea - mPosition.y) / mVitesse.y;
            tmp = (mArea2 * sizeArea - mPosition.y - mEpaisseur) / mVitesse.y;
        }
    }
    else if (gravity.y < 0.0)
    {
        result = Solveur::fstQuadratique(gravity.y / 2.0, mVitesse.y, mPosition.y - (mArea1 + 1) * sizeArea);
        tmp = Solveur::sndQuadratique(gravity.y / 2.0, mVitesse.y, mPosition.y - mArea1 * sizeArea);
        if (tmp < result)
            result = tmp;
        tmp = Solveur::fstQuadratique(gravity.y / 2.0, mVitesse.y, mPosition.y + mEpaisseur - (mArea2 + 1) * sizeArea);
        if (tmp < result)
            result = tmp;
        tmp = Solveur::sndQuadratique(gravity.y / 2.0, mVitesse.y, mPosition.y + mEpaisseur - mArea2 * sizeArea);
    }
    else if (gravity.y > 0.0)
    {
        result = Solveur::sndQuadratique(gravity.y / 2.0, mVitesse.y, mPosition.y - (mArea1 + 1) * sizeArea);
        tmp = Solveur::fstQuadratique(gravity.y / 2.0, mVitesse.y, mPosition.y - mArea1 * sizeArea);
        if (tmp < result)
            result = tmp;
        tmp = Solveur::sndQuadratique(gravity.y / 2.0, mVitesse.y, mPosition.y + mEpaisseur - (mArea2 + 1) * sizeArea);
        if (tmp < result)
            result = tmp;
        tmp = Solveur::fstQuadratique(gravity.y / 2.0, mVitesse.y, mPosition.y + mEpaisseur - mArea2 * sizeArea);
    }
    if (tmp < result)
        result = tmp;

    return result;
}


// Effectue la collision avec le mobile.
void Piston::doCollision(Mobile* mobile, State& state)
{
    mobile->doCollision(this, state);
}

// Collision avec une boule.
void Piston::doCollision(Boule* boule, State& state)
{
    boule->doCollision(this, state);
}

// Collision avec un piston.
void Piston::doCollision(Piston* piston, State& state)
{
    // Vitesses initiales.
    double vitesse1 = mVitesse.y;
    double vitesse2 = piston->mVitesse.y;

    // Les mobiles se font face.
    if ((vitesse1 - vitesse2) * (mPosition.y - piston->mPosition.y) < 0.0)
    {
        // Changement des vitesses selon les masses.
        mVitesse.y = (vitesse1 * (mMasse - piston->mMasse) + vitesse2 * 2.0 * piston->mMasse) / (mMasse + piston->mMasse);
        piston->mVitesse.y = (vitesse2 * (piston->mMasse - mMasse) + vitesse1 * 2.0 * mMasse) / (mMasse + piston->mMasse);

        this->updateRefresh(state);
        piston->updateRefresh(state);
    }
    else
    {
        state.toRefresh.insert(this);
        state.toRefresh.insert(piston);
    }
}

// Effectue un changement de zone.
void Piston::changeArea(State& state)
{
    state.mapMobiles[mArea1].pistons().erase(mMapIt1);
    state.mapMobiles[mArea2].pistons().erase(mMapIt2);

    // Calcul des zones.
    if (mVitesse.y >= 0)
    {
        mArea1 = std::floor(mPosition.y / state.sizeArea + 0.5);
        mArea2 = std::floor((mPosition.y + mEpaisseur) / state.sizeArea + 0.5);
    }
    else
    {
        mArea1 = std::floor(mPosition.y / state.sizeArea - 0.5);
        mArea2 = std::floor((mPosition.y + mEpaisseur) / state.sizeArea - 0.5);
    }

    // Mise à jour de la carte.
    auto& pistons1 = state.mapMobiles[mArea1].pistons();
    pistons1.prepend(this);
    mMapIt1 = pistons1.begin();

    auto& pistons2 = state.mapMobiles[mArea2].pistons();
    pistons2.prepend(this);
    mMapIt2 = pistons2.begin();

    this->updateRefresh(state);
}


// Cherche des collisions avec des mobiles.
void Piston::updateCollisionsMobiles(State& state)
{
    // Vérifie les mobiles des zones voisines.
    for (int j = mArea1 - 1 ; j <= mArea1 + 1 ; ++j)
    {
        // Vérifie les boules.
        for (auto& boule : state.mapMobiles[j].boules())
            this->testeCollision(boule, state);

        // Vérifie les pistons.
        for (auto& piston : state.mapMobiles[j].pistons())
            if (piston != this)
                this->testeCollision(piston, state);
    }

    // Vérifie les mobiles des zones voisines.
    for (int j = mArea2 - 1 ; j <= mArea2 + 1 ; ++j)
    {
        // Vérifie les boules.
        for (auto& boule : state.mapMobiles[j].boules())
            this->testeCollision(boule, state);

        // Vérifie les pistons.
        for (auto& piston : state.mapMobiles[j].pistons())
            if (piston != this)
                this->testeCollision(piston, state);
    }

    // Vérifie un changement de zone.
    this->testeCollision(Collision(this), state);
}
