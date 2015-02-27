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

#include "boule.hpp"

#include "coord_io.tpl"
#include "state.hpp"

// Constructeur.
Boule::Boule(const Coord<double>& position, const Coord<double>& vitesse, const QColor& color, double masse, double rayon, State& state) :
    Mobile(position, vitesse, color, masse),
    mOrigine(position),
    mOldFree(std::make_pair(Coord<double>(), Time())),
    mLastFree(std::make_pair(position, state.now)),
    mRayon(rayon),
    mArea(std::floor(position.x / state.sizeArea), std::floor(position.y / state.sizeArea)),
    mMapIt(state.mapMobiles[mArea.y].boules().insert(mArea.x, this))
{
}


// Avance le mobile jusqu'à l'instant indiqué (sans tenir compte des autres mobiles).
void Boule::avance(const Time& time, const Coord<double>& gravity)
{
    mPosition += mVitesse * time.time() + gravity * time.time() * time.time() / 2;
    mVitesse += gravity * time.time();
}


// Effectue une mutation pour cette boule.
void Boule::changePopulation(State& state)
{
    for (auto& mutation : state.config.configMutations())
    {
        if (mPopulation == mutation.mPop1)
        {
            this->swap(mutation.mPop2, state, true);
            return;
        }
    }
}

// Définit la population de cette boule et prépare la prochaine mutation.
void Boule::setPopulation(unsigned int population, State& state)
{
    mPopulation = population;
    mEventIt = state.events.end();

    for (auto& mutation : state.config.configMutations())
    {
        if (mPopulation == mutation.mPop1)
        {
            double time = mutation.mTau;

            if (mutation.mType == ConfigReaction::proba)
            {
                std::exponential_distribution<> distrib(1.0 / time);
                time = distrib(Solveur::generateur);
            }

            if (time > 0)
                mEventIt = state.events.insert(
                            std::make_pair(
                                state.now + Time(time),
                                std::make_shared<BouleEvent>(this)
                            ));
            break;
        }
    }
}


// Calcule l'instant de la prochaine collision avec le mobile.
Time Boule::collision(const Mobile* mobile) const
{
    return mobile->collision(this);
}

Time Boule::collision(const Piston* piston) const
{
    // Différence de vitesse.
    double dVitesseY = piston->mVitesse.y - mVitesse.y;
    double dPositionY = mPosition.y - piston->mPosition.y;

    // Les mobiles s'éloignent.
    if (dVitesseY * dPositionY < 0.0)
        return Time();

    // Différence de position.
    if (dPositionY >= 0.0)
        dPositionY -= (mRayon + piston->epaisseur());
    else
        dPositionY += mRayon;

    // Calcul de l'instant.
    return Time(dPositionY / dVitesseY);
}

Time Boule::collision(const Boule* boule) const
{
    // Différence de vitesse et position.
    Coord<double> dVitesse = mVitesse - boule->mVitesse;
    Coord<double> dPosition = mPosition - boule->mPosition;

    // Somme des rayons.
    double rayons = mRayon + boule->mRayon;

    // Résolution d'une équation du second degré.
    return Solveur::fstQuadratique(
                dVitesse.squareLength(),
                2.0 * dVitesse.scalar(dPosition),
                dPosition.squareLength() - rayons * rayons
                );
}

// Calcule l'instant de la prochaine collision avec l'obstacle.
Time Boule::collision(const Coord<double>& sommet, const Coord<double>& gravity) const
{
    // Différence de position.
    Coord<double> dPosition = mPosition - sommet;

    // Avec gravité : équation de degré 4.
    if (gravity != Coord<double>())
        return Solveur::fstQuartique(
                    gravity.squareLength(),
                    2.0 * gravity.scalar(mVitesse),
                    mVitesse.squareLength() + 2.0 * gravity.scalar(dPosition),
                    2.0 * mVitesse.scalar(dPosition),
                    dPosition.squareLength() - mRayon * mRayon
                    );
    // Sans gravité : équation du second degré.
    else
        return Solveur::fstQuadratique(
                    mVitesse.squareLength(),
                    2.0 * mVitesse.scalar(dPosition),
                    dPosition.squareLength() - mRayon * mRayon
                    );
}

Time Boule::collision(const Segment& segment, const Coord<double>& gravity) const
{
    // Différence de position.
    Coord<double> dPosition(segment.vect(mPosition));

    // Calcul de l'orientation des dérivées de dPosition relativement au segment.
    double det = dPosition.det(segment.vect());
    double detgrav = gravity.det(segment.vect()) / 2.0;
    double detvit = mVitesse.det(segment.vect());

    double direction = det * (detgrav == 0.0 ? detvit : detgrav);

    // Côté du segment où se trouve la boule.
    if (det > 0)
        det -= mRayon * segment.length();
    else
        det += mRayon * segment.length();

    // Instant de collision avec le segment.
    Time result;
    // Gravité parallèle au segment : équation du premier degré.
    if (detgrav == 0.0)
        result = (direction > 0.0 ? Time() : -det / detvit);
    // Equation du second degré (première ou seconde solution selon le signe).
    else if (direction < 0.0)
        result = Solveur::sndQuadratique(detgrav, detvit, det);
    else
        result = Solveur::fstQuadratique(detgrav, detvit, det);

    // Position au moment de la collision.
    Coord<double> future(mPosition + mVitesse * result.time() + gravity * result.time() * result.time() / 2.0);

    // Vérification que la collision sera dans l'intervalle du segment.
    if (segment.face(future))
        return result;
    return Time();
}

// Calcule l'instant du prochain changement de zone.
Time Boule::newArea(double sizeArea, const Coord<double>& gravity) const
{
    Time result;
    Time tmp;

    // Résolution selon le signe de la gravité.
    if (gravity.x == 0.0)
    {
        if (mVitesse.x > 0)
            result = ((mArea.x + 1) * sizeArea - mPosition.x) / mVitesse.x;
        else if (mVitesse.x < 0)
            result = (mArea.x * sizeArea - mPosition.x) / mVitesse.x;
    }
    else if (gravity.x < 0.0)
    {
        result = Solveur::fstQuadratique(gravity.x / 2.0, mVitesse.x, mPosition.x - (mArea.x + 1) * sizeArea);
        tmp = Solveur::sndQuadratique(gravity.x / 2.0, mVitesse.x, mPosition.x - mArea.x * sizeArea);
    }
    else if (gravity.x > 0.0)
    {
        result = Solveur::sndQuadratique(gravity.x / 2.0, mVitesse.x, mPosition.x - (mArea.x + 1) * sizeArea);
        tmp = Solveur::fstQuadratique(gravity.x / 2.0, mVitesse.x, mPosition.x - mArea.x * sizeArea);
    }
    // Choix du plus proche temps.
    if (tmp < result)
        result = tmp;

    // Résolution selon le signe de la gravité.
    if (gravity.y == 0.0)
    {
        if (mVitesse.y > 0)
            tmp = ((mArea.y + 1) * sizeArea - mPosition.y) / mVitesse.y;
        else if (mVitesse.y < 0)
            tmp = ((mArea.y) * sizeArea - mPosition.y) / mVitesse.y;
    }
    else if (gravity.y < 0.0)
    {
        tmp = Solveur::fstQuadratique(gravity.y / 2.0, mVitesse.y, mPosition.y - (mArea.y + 1) * sizeArea);
        if (tmp < result)
            result = tmp;
        tmp = Solveur::sndQuadratique(gravity.y / 2.0, mVitesse.y, mPosition.y - mArea.y * sizeArea);
    }
    else if (gravity.y > 0.0)
    {
        tmp = Solveur::sndQuadratique(gravity.y / 2.0, mVitesse.y, mPosition.y - (mArea.y + 1) * sizeArea);
        if (tmp < result)
            result = tmp;
        tmp = Solveur::fstQuadratique(gravity.y / 2.0, mVitesse.y, mPosition.y - mArea.y * sizeArea);
    }
    // Choix du plus proche temps.
    if (tmp < result)
        result = tmp;

    return result;
}


// Effectue la collision avec le mobile.
void Boule::doCollision(Mobile* mobile, State& state)
{
    mobile->doCollision(this, state);
}

// Collision avec une boule.
void Boule::doCollision(Boule* boule, State& state)
{
    // Différence de position.
    Coord<double> dPosition = boule->mPosition - mPosition;
    Coord<double> dVitesse = boule->mVitesse - mVitesse;

    // Les mobiles se font face.
    if (dPosition.scalar(dVitesse) < 0.0)
    {
        // Met à jour l'instant de la dernière collision.
        mOldFree = mLastFree;
        mLastFree.first = mPosition;
        mLastFree.second = state.now;
        boule->mLastFree.first = boule->mPosition;
        boule->mLastFree.second = state.now;

        dPosition /= dPosition.length();

        // Calcul des vitesses dans le repère orthonormé basé sur le vecteur dPosition.
        Coord<double> vitesse1 = Coord<double>(mVitesse.scalar(dPosition), dPosition.det(mVitesse));
        Coord<double> vitesse2 = Coord<double>(boule->mVitesse.scalar(dPosition), dPosition.det(boule->mVitesse));

        // Changement des vitesses selon les masses.
        mVitesse = (dPosition * (vitesse1.x * (mMasse - boule->mMasse) + vitesse2.x * 2.0 * boule->mMasse) / (mMasse + boule->mMasse)
                     + Coord<double>(-dPosition.y, dPosition.x) * vitesse1.y);
        boule->mVitesse = (dPosition * (vitesse2.x * (boule->mMasse - mMasse) + vitesse1.x * 2.0 * mMasse) / (mMasse + boule->mMasse)
                     + Coord<double>(-dPosition.y, dPosition.x) * vitesse2.y);

        this->updateRefresh(state);
        boule->updateRefresh(state);
    }
    // Erreur : les mobiles s'éloignent !
    else
    {
        std::cerr << "rejected collision (balls) : " << *this << " ; " << *boule << std::endl;
        state.toRefresh.insert(this);
        state.toRefresh.insert(boule);
    }

    // Changements de populations issus du contact entre les boules (réactions).
    for (auto& reaction : state.config.configReactions())
    {
        if ((mPopulation == reaction.mPop1 && boule->mPopulation == reaction.mPop2)
         || (mPopulation == reaction.mPop2 && boule->mPopulation == reaction.mPop1))
        {
            if (reaction.mType == ConfigReaction::proba)
            {
                std::bernoulli_distribution distrib(reaction.mSeuil);
                if (!distrib(Solveur::generateur))
                    break;
            }

            if (mPopulation == reaction.mPop1)
            {
                this->swap(reaction.m_Pop1, state, true);
                boule->swap(reaction.m_Pop2, state, true);
            }
            else
            {
                this->swap(reaction.m_Pop2, state, true);
                boule->swap(reaction.m_Pop1, state, true);
            }

            break;
        }
    }
}

// Collision avec un piston.
void Boule::doCollision(Piston* piston, State& state)
{
    // Vitesses initiales.
    double vitesse1 = mVitesse.y;
    double vitesse2 = piston->mVitesse.y;

    // Les mobiles se font face.
    if ((vitesse1 - vitesse2) * (mPosition.y - piston->mPosition.y) < 0.0)
    {
        // Met à jour l'instant de la dernière collision.
        mOldFree = mLastFree;
        mLastFree.first = mPosition;
        mLastFree.second = state.now;

        // Changement des vitesses selon les masses.
        mVitesse.y = (vitesse1 * (mMasse - piston->mMasse) + vitesse2 * 2.0 * piston->mMasse) / (mMasse + piston->mMasse);
        piston->mVitesse.y = (vitesse2 * (piston->mMasse - mMasse) + vitesse1 * 2.0 * mMasse) / (mMasse + piston->mMasse);

        this->updateRefresh(state);
        piston->updateRefresh(state);
    }
    // Erreur : les mobiles s'éloignent !
    else
    {
        std::cerr << "rejected collision (piston) : " << *this << " ; " << *piston << std::endl;
        state.toRefresh.insert(this);
        state.toRefresh.insert(piston);
    }
}

// Collision avec un sommet.
void Boule::doCollision(const Coord<double>& sommet, State& state)
{
    // Différence de position.
    Coord<double> dPosition = sommet - mPosition;

    // La boule fait face au sommet.
    if (dPosition.scalar(mVitesse) > 0.0)
    {
        // Met à jour l'instant de la dernière collision.
        mOldFree = mLastFree;
        mLastFree.first = mPosition;
        mLastFree.second = state.now;

        // Changement de vitesse selon l'axe [centre boule -- choc].
        dPosition /= dPosition.length();
        mVitesse = dPosition * mVitesse.scalar(-dPosition)
                + Coord<double>(-dPosition.y, dPosition.x) * dPosition.det(mVitesse);

        this->updateRefresh(state);
    }
    // Erreur : la boule s'éloigne du sommet !
    else
    {
        std::cerr << "rejected collision (vertex) : " << *this << std::endl;
        state.toRefresh.insert(this);
    }
}

// Collision avec un segment.
void Boule::doCollision(const Segment& segment, State& state)
{
    Coord<double> vect = segment.vect();

    // La boule fait face au segment.
    if (vect.det(segment.vect(mPosition)) * vect.det(mVitesse) < 0.0)
    {
        // Met à jour l'instant de la dernière collision.
        mOldFree = mLastFree;
        mLastFree.first = mPosition;
        mLastFree.second = state.now;

        // Changement de vitesse selon l'axe orthogonal au segment.
        vect /= vect.length();
        mVitesse = vect * vect.scalar(mVitesse)
                + Coord<double>(vect.y, -vect.x) * vect.det(mVitesse);
        this->updateRefresh(state);
    }
    // Erreur : la boule s'éloigne du segment !
    else
    {
        std::cerr << "rejected collision (segment) : " << *this << std::endl;
        state.toRefresh.insert(this);
    }
}

// Effectue un changement de zone.
void Boule::changeArea(State& state)
{
    this->detachArea(state);
    this->setArea(state);
}


// Recalcule la zone et met à jour la table des zones.
void Boule::setArea(State& state)
{
    double sizeArea = state.sizeArea;
    bool x = (std::fabs(std::round(mPosition.x / sizeArea) - mPosition.x / sizeArea) <= std::fabs(std::round(mPosition.y / sizeArea) - mPosition.y / sizeArea));

    // Calcul des zones
    if (x)
    {
        if (mVitesse.x >= 0)
            mArea.x = std::floor(mPosition.x / sizeArea + 0.5);
        else
            mArea.x = std::floor(mPosition.x / sizeArea - 0.5);
    }
    else
    {
        if (mVitesse.y >= 0)
            mArea.y = std::floor(mPosition.y / sizeArea + 0.5);
        else
            mArea.y = std::floor(mPosition.y / sizeArea - 0.5);
    }

    // Mise à jour de la table.
    mMapIt = state.mapMobiles[mArea.y].boules().insert(mArea.x, this);
    this->updateRefresh(state);
}


// Cherche des collisions avec des mobiles.
void Boule::updateCollisionsMobiles(State& state)
{
    // Ensemble des segments trouvés dans les zones voisines.
    std::set<Segment> segments;

    // Vérifie les mobiles des zones voisines.
    for (int j = mArea.y - 1 ; j <= mArea.y + 1 ; ++j)
    {
        //auto& mapj = state.mapMobiles[j];

        for (int i = mArea.x - 1 ; i <= mArea.x + 1 ; ++i)
        {
            // Vérifie les boules.
            for (auto& boule : state.mapMobiles[j].boules().values(i))
                if (boule != this)
                    this->testeCollision(boule, state);

            // Vérifie les sommets.
            for (auto& sommet : state.mapMobiles[j].sommets().values(i))
                this->testeCollision(Collision(this, sommet), state);

            // Ajoute les segments à l'ensemble à traiter (un segment peut être sur plusieurs zones).
            for (auto& segment : state.mapMobiles[j].segments().value(i))
                segments.insert(segment);
        }

        // Vérifie les pistons.
        for (auto& piston : state.mapMobiles[j].pistons())
            this->testeCollision(piston, state);
    }

    // Vérifie les segments listés.
    for (auto& segment : segments)
        this->testeCollision(Collision(this, segment), state);

    // Vérifie un changement de zone.
    this->testeCollision(Collision(this), state);
}

// Enlève la boule de la table des zones.
void Boule::detachArea(State& state)
{
    state.mapMobiles[mArea.y].boules().erase(mMapIt);
}


// Change la boule de population.
void Boule::swap(unsigned int population, State& state, bool eraseEvent)
{
    if (population == mPopulation)
        return;

    // Change la boule de population.
    state.populations[mPopulation].boules().erase(this);

    Population& pop = state.populations[population];
    pop.boules().insert(this);
    mColor = pop.color();

    // Met à jour les événements (mutations).
    if (eraseEvent && mEventIt != state.events.end())
        state.events.erase(mEventIt);
    this->setPopulation(population, state);
}
