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

#include "boule.hpp"

#include "coord_io.tpl"
#include "population.hpp"
#include "collision.hpp"
#include "piston.hpp"
#include "solveur.hpp"
#include "map_ligne.hpp"

// Constructeur.
Boule::Boule(const Coord<double>& position, const Coord<double>& vitesse, const QColor& color, double masse, double rayon, const Time& now, double sizeArea, QMap<int, MapLigne>& mapMobiles) :
    Mobile(position, vitesse, color, masse),
    mOrigine(position),
    mOldFree(std::make_pair(Coord<double>(), Time())),
    mLastFree(std::make_pair(position, now)),
    mRayon(rayon),
    mArea(floor(position.x / sizeArea), floor(position.y / sizeArea)),
    mMapIt(mapMobiles[mArea.y].boules().insert(mArea.x, this))
{
}


// Avance le mobile jusqu'à l'instant indiqué (sans tenir compte des autres mobiles).
void Boule::avance(const Time& time, const Coord<double>& gravity)
{
    mPosition += mVitesse * time.time() + gravity * time.time() * time.time() / 2;
    mVitesse += gravity * time.time();
}


// Effectue une mutation pour cette boule.
void Boule::changePopulation(const Time& now, QList<Population>& populations, std::multimap<Time, std::shared_ptr<Event> >& events, const QList<ConfigMutation>& configMutations)
{
    for (int i = 0 ; i < configMutations.size() ; ++i)
    {
        const ConfigMutation& mutation = configMutations[i];
        if (mPopulation == mutation.mPop1)
        {
            this->swap(now, mutation.mPop2, populations, events, configMutations, true);
            return;
        }
    }
}

// Définit la population de cette boule et prépare la prochaine mutation.
void Boule::setPopulation(const Time& now, unsigned int population, std::list<std::shared_ptr<Boule> >::iterator iterator, std::multimap<Time, std::shared_ptr<Event> >& events, const QList<ConfigMutation>& configMutations)
{
    mPopulation = population;
    mPopIt = iterator;

    mEventIt = events.end();

    for (int i = 0 ; i < configMutations.size() ; ++i)
    {
        const ConfigMutation& mutation = configMutations[i];
        if (mPopulation == mutation.mPop1)
        {
            double time = mutation.mTau;

            if (mutation.mType == ConfigReaction::proba)
            {
                std::exponential_distribution<> distrib(1.0 / time);
                time = distrib(Solveur::generateur);
            }

            if (time > 0)
                mEventIt = events.insert(std::make_pair(now + Time(time), std::make_shared<BouleEvent>(this)));
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
    return Solveur::fstQuadratique(dVitesse.squareLength(), 2.0 * dVitesse.scalar(dPosition), dPosition.squareLength() - rayons * rayons);
}

// Calcule l'instant de la prochaine collision avec l'obstacle.
Time Boule::collision(const Coord<double>& sommet, const Coord<double>& gravity) const
{
    // Différence de position.
    Coord<double> dPosition = mPosition - sommet;

    // Avec gravité : équation de degré 4.
    if (gravity != Coord<double>())
        return Solveur::fstQuartique(gravity.squareLength(), 2.0 * gravity.scalar(mVitesse), mVitesse.squareLength() + 2.0 * gravity.scalar(dPosition), 2.0 * mVitesse.scalar(dPosition), dPosition.squareLength() - mRayon * mRayon);
    // Sans gravité : équation du second degré.
    else
        return Solveur::fstQuadratique(mVitesse.squareLength(), 2.0 * mVitesse.scalar(dPosition), dPosition.squareLength() - mRayon * mRayon);
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
void Boule::doCollision(const Time& now, Mobile* mobile, std::set<Mobile*>& toRefresh, std::multimap<Time, std::shared_ptr<Event> >& events, const QList<ConfigMutation>& configMutations, const QList<ConfigReaction>& configReactions, QList<Population>& populations, std::pair<unsigned int, unsigned int>& countEtudes)
{
    mobile->doCollision(now, this, toRefresh, events, configMutations, configReactions, populations, countEtudes);
}

// Collision avec une boule.
void Boule::doCollision(const Time& now, Boule* boule, std::set<Mobile*>& toRefresh, std::multimap<Time, std::shared_ptr<Event> >& events, const QList<ConfigMutation>& configMutations, const QList<ConfigReaction>& configReactions, QList<Population>& populations, std::pair<unsigned int, unsigned int>& countEtudes)
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
        mLastFree.second = now;
        boule->mLastFree.first = boule->mPosition;
        boule->mLastFree.second = now;

        dPosition /= dPosition.length();

        // Calcul des vitesses dans le repère orthonormé basé sur le vecteur dPosition.
        Coord<double> vitesse1 = Coord<double>(mVitesse.scalar(dPosition), dPosition.det(mVitesse));
        Coord<double> vitesse2 = Coord<double>(boule->mVitesse.scalar(dPosition), dPosition.det(boule->mVitesse));

        // Changement des vitesses selon les masses.
        mVitesse = (dPosition * (vitesse1.x * (mMasse - boule->mMasse) + vitesse2.x * 2.0 * boule->mMasse) / (mMasse + boule->mMasse)
                     + Coord<double>(-dPosition.y, dPosition.x) * vitesse1.y);
        boule->mVitesse = (dPosition * (vitesse2.x * (boule->mMasse - mMasse) + vitesse1.x * 2.0 * mMasse) / (mMasse + boule->mMasse)
                     + Coord<double>(-dPosition.y, dPosition.x) * vitesse2.y);

        this->updateRefresh(toRefresh);
        boule->updateRefresh(toRefresh);
    }
    // Erreur : les mobiles s'éloignent !
    else
    {
        //std::cout << "collision refusée (boules) : " << *this << " ; " << *boule << std::endl;
        toRefresh.insert(this);
        toRefresh.insert(boule);
    }

    // Changements de populations issus du contact entre les boules (réactions).
    for (int i = 0 ; i < configReactions.size() ; ++i)
    {
        const ConfigReaction& reaction = configReactions[i];
        if ((mPopulation == reaction.mPop1 && boule->mPopulation == reaction.mPop2) || (mPopulation == reaction.mPop2 && boule->mPopulation == reaction.mPop1))
        {
            if (reaction.mType == ConfigReaction::proba)
            {
                std::bernoulli_distribution distrib(reaction.mSeuil);
                if (!distrib(Solveur::generateur))
                    break;
            }

            if (mPopulation == reaction.mPop1)
            {
                this->swap(now, reaction.m_Pop1, populations, events, configMutations, true);
                boule->swap(now, reaction.m_Pop2, populations, events, configMutations, true);
            }
            else
            {
                this->swap(now, reaction.m_Pop2, populations, events, configMutations, true);
                boule->swap(now, reaction.m_Pop1, populations, events, configMutations, true);
            }

            break;
        }
    }
}

// Collision avec un piston.
void Boule::doCollision(const Time& now, Piston* piston, std::set<Mobile*>& toRefresh, std::multimap<Time, std::shared_ptr<Event> >&/* events*/, const QList<ConfigMutation>&/* configMutations*/, const QList<ConfigReaction>&/* configReactions*/, QList<Population>&/* populations*/, std::pair<unsigned int, unsigned int>& countEtudes)
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
        mLastFree.second = now;

        // Changement des vitesses selon les masses.
        mVitesse.y = (vitesse1 * (mMasse - piston->mMasse) + vitesse2 * 2.0 * piston->mMasse) / (mMasse + piston->mMasse);
        piston->mVitesse.y = (vitesse2 * (piston->mMasse - mMasse) + vitesse1 * 2.0 * mMasse) / (mMasse + piston->mMasse);

        this->updateRefresh(toRefresh);
        piston->updateRefresh(toRefresh);
    }
    // Erreur : les mobiles s'éloignent !
    else
    {
        std::cout << "collision refusée (piston) : " << *this << " ; " << *piston << std::endl;
        toRefresh.insert(this);
        toRefresh.insert(piston);
    }
}

// Collision avec un sommet.
void Boule::doCollision(const Time& now, const Coord<double>& sommet, std::set<Mobile*>& toRefresh, std::pair<unsigned int, unsigned int>& countEtudes)
{
    // Différence de position.
    Coord<double> dPosition = sommet - mPosition;

    // La boule fait face au sommet.
    if (dPosition.scalar(mVitesse) > 0.0)
    {
        // Met à jour l'instant de la dernière collision.
        mOldFree = mLastFree;
        mLastFree.first = mPosition;
        mLastFree.second = now;

        // Changement de vitesse selon l'axe [centre boule -- choc].
        dPosition /= dPosition.length();
        mVitesse = dPosition * mVitesse.scalar(-dPosition) + Coord<double>(-dPosition.y, dPosition.x) * dPosition.det(mVitesse);

        this->updateRefresh(toRefresh);
    }
    // Erreur : la boule s'éloigne du sommet !
    else
    {
        std::cout << "collision refusée (sommet) : " << *this << std::endl;
        toRefresh.insert(this);
    }
}

// Collision avec un segment.
void Boule::doCollision(const Time& now, const Segment& segment, std::set<Mobile*>& toRefresh, std::pair<unsigned int, unsigned int>& countEtudes)
{
    Coord<double> vect = segment.vect();

    // La boule fait face au segment.
    if (vect.det(segment.vect(mPosition)) * vect.det(mVitesse) < 0.0)
    {
        // Met à jour l'instant de la dernière collision.
        mOldFree = mLastFree;
        mLastFree.first = mPosition;
        mLastFree.second = now;

        // Changement de vitesse selon l'axe orthogonal au segment.
        vect /= vect.length();
        mVitesse = vect * vect.scalar(mVitesse) + Coord<double>(vect.y, -vect.x) * vect.det(mVitesse);
        this->updateRefresh(toRefresh);
    }
    // Erreur : la boule s'éloigne du segment !
    else
    {
        std::cout << "collision refusée (segment) : " << *this << std::endl;
        toRefresh.insert(this);
    }
}

// Effectue un changement de zone.
void Boule::changeArea(double sizeArea, std::set<Mobile*>& toRefresh, QMap<int, MapLigne>& mapMobiles, std::pair<unsigned int, unsigned int>& countEtudes)
{
    this->detachArea(mapMobiles);
    this->setArea(sizeArea, toRefresh, mapMobiles);
}


// Recalcule la zone et met à jour la table des zones.
void Boule::setArea(double sizeArea, std::set<Mobile*>& toRefresh, QMap<int, MapLigne>& mapMobiles)
{
    bool x = (fabs(round(mPosition.x / sizeArea) - mPosition.x / sizeArea) <= fabs(round(mPosition.y / sizeArea) - mPosition.y / sizeArea));

    // Calcul des zones
    if (x)
    {
        if (mVitesse.x >= 0)
            mArea.x = floor(mPosition.x / sizeArea + 0.5);
        else
            mArea.x = floor(mPosition.x / sizeArea - 0.5);
    }
    else
    {
        if (mVitesse.y >= 0)
            mArea.y = floor(mPosition.y / sizeArea + 0.5);
        else
            mArea.y = floor(mPosition.y / sizeArea - 0.5);
    }

    // Mise à jour de la table.
    mMapIt = mapMobiles[mArea.y].boules().insert(mArea.x, this);
    this->updateRefresh(toRefresh);
}


// Cherche des collisions avec des mobiles.
void Boule::updateCollisionsMobiles(std::multimap<Time, std::shared_ptr<Event> >& events, QMap<int, MapLigne>& mapMobiles, const Time& now, double sizeArea, const Coord<double>& gravity, std::pair<unsigned int, unsigned int>& countEtudes)
{
    // Ensemble des segments trouvés dans les zones voisines.
    std::set<Segment> segments;

    // Vérifie les mobiles des zones voisines.
    for (int j = mArea.y - 1 ; j <= mArea.y + 1 ; ++j)
    {
        for (int i = mArea.x - 1 ; i <= mArea.x + 1 ; ++i)
        {
            // Vérifie les boules.
            QList<Boule*> list = mapMobiles[j].boules().values(i);

            for (int k = 0 ; k < list.size() ; ++k)
                if (list[k] != this)
                    this->testeCollision(list[k], events, now, sizeArea, gravity, countEtudes);

            // Vérifie les sommets.
            QList<Coord<double> > sommets = mapMobiles[j].sommets().values(i);

            for (int k = 0 ; k < sommets.size() ; ++k)
                this->testeCollision(Collision(this, sommets[k]), events, now, sizeArea, gravity, countEtudes);

            // Ajoute les segments à l'ensemble.
            std::set<Segment> sgts = mapMobiles[j].segments().value(i);
            for (auto it = sgts.begin() ; it != sgts.end() ; ++it)
                segments.insert(*it);
        }

        // Vérifie les pistons.
        QList<Piston*> list = mapMobiles[j].pistons();

        for (int k = 0 ; k < list.size() ; ++k)
            this->testeCollision(list[k], events, now, sizeArea, gravity, countEtudes);
    }

    // Vérifie les segments listés.
    for (auto it = segments.begin() ; it != segments.end() ; ++it)
        this->testeCollision(Collision(this, *it), events, now, sizeArea, gravity, countEtudes);

    // Vérifie un changement de zone.
    this->testeCollision(Collision(this), events, now, sizeArea, gravity, countEtudes);
}

// Enlève la boule de la table des zones.
void Boule::detachArea(QMap<int, MapLigne>& mapMobiles)
{
    mapMobiles[mArea.y].boules().erase(mMapIt);
}


// Change la boule de population.
void Boule::swap(const Time& now, unsigned int population, QList<Population>& populations, std::multimap<Time, std::shared_ptr<Event> >& events, const QList<ConfigMutation>& configMutations, bool eraseEvent)
{
    if (population == mPopulation)
        return;

    // Supprime la boule de la population.
    std::shared_ptr<Boule> value = *mPopIt;
    populations[mPopulation].boules().erase(mPopIt);

    // Ajoute la boule à la population.
    mPopIt = populations[population].boules().insert(populations[population].boules().end(), value);
    mColor = populations[population].color();

    if (eraseEvent && mEventIt != events.end())
        events.erase(mEventIt);
    this->setPopulation(now, population, mPopIt, events, configMutations);
}
