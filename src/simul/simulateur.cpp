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

#include "simulateur.hpp"

#include <QPainter>
#include <QCoreApplication>
#include "coord_io.tpl"

// Constructeur.
Simulateur::Simulateur(const Configuration& config) :
    mLayout(new QGridLayout(this)),
    mGroupCourbes(new CourbesGroup(500)),
    mLabelVitesse(new QLabel("simulation speed :")),
    mSliderVitesse(new QSlider(Qt::Horizontal)),
    mLabelValues(new QLabel("measure frequency :")),
    mSliderValues(new QSlider(Qt::Horizontal)),
    mLabelCourbes(new QLabel("display frequency :")),
    mSliderCourbes(new QSlider(Qt::Horizontal)),
    mState(config)
{
    // Création de l'interface graphique.
    mSliderVitesse->setRange(-1000, 250);
    mSliderValues->setRange(-500, 500);
    mSliderCourbes->setRange(-750, 250);

    mLayout->setMargin(0);
    mLayout->addWidget(mGroupCourbes, 0, 0, 1, 2);
    mLayout->addWidget(mLabelVitesse, 1, 0);
    mLayout->addWidget(mSliderVitesse, 1, 1);
    mLayout->addWidget(mLabelValues, 2, 0);
    mLayout->addWidget(mSliderValues, 2, 1);
    mLayout->addWidget(mLabelCourbes, 3, 0);
    mLayout->addWidget(mSliderCourbes, 3, 1);

    // Connexion des signaux et slots.
    QObject::connect(mSliderVitesse, SIGNAL(valueChanged(int)), this, SLOT(setVitesse(int)));
    QObject::connect(mSliderValues, SIGNAL(valueChanged(int)), this, SLOT(setValues(int)));
    QObject::connect(mSliderCourbes, SIGNAL(valueChanged(int)), this, SLOT(setCourbes(int)));

    // Initialisation.
    mSliderVitesse->setValue(-500);
    mSliderValues->setValue(0);
    mSliderCourbes->setValue(-250);

    this->setValues(0);
}


// Redémarre la simulation.
void Simulateur::doRestart()
{
    // Destruction de la simulation précédente.
    mState.clear();
    mGroupCourbes->clear();

    mState.create();

    // Création des courbes.
    for (auto& fcourbe : mState.config.configFcourbes())
        mGroupCourbes->addCourbe(fcourbe);

    // Création des profils.
    for (auto& profil : mState.config.configProfils())
        mGroupCourbes->addProfil(profil);

    // Ajoute les événements de dessin et de courbe.
    this->addDrawEvent();
    this->addValueEvent();
    this->addCourbeEvent();

    // Redessine l'espace.
    emit fullDraw();
}

// Avance jusqu'au prochain événement de dessin.
void Simulateur::playToNextDraw()
{
    while (!this->playNext());
}

// Avance jusqu'au prochain événement et effectue tous les événements de cette date.
bool Simulateur::playNext()
{
    // Avance jusqu'au prochain événement.
    Time timeEvent = mState.events.begin()->first;
    this->avance(timeEvent);

    //*
    bool isDraw = false;
    for (auto it = mState.events.begin() ; it != mState.events.end() && it->first == mState.now ; ++it)
        if (it->second->perform(*this, isDraw))
            mState.drawingsRefresh.push_back(it);
    /*/
    unsigned int count = 0;
    unsigned int countChocs = 0;
    for (auto it = mEvents.begin() ; it != mEvents.end() && it->first == mNow ; ++it, ++count)
    {
        ++countChocs;
        if (it->second->perform(*this))
        {
            mDrawingsRefresh.push_back(it);
            --countChocs;
        }
    }

    if (countChocs > 1)
        std::cout << "Plusieurs collisions simultanées !" << std::endl;
    if (count > 1)
        std::cout << "Plusieurs dessins simultanés !" << std::endl;
        //QMessageBox::warning(this, "warning", "Several simultaneous collisions !");
    //*/

    // Met à jour les événements de collisions et de dessin.
    this->refreshCollisions();
    this->refreshDrawings();

    return isDraw;
}


// Dessine l'état actuel de la simulation.
void Simulateur::draw(QPainter& painter, double width)
{
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    // Détermine les coordonnées spatiales des côtés de la fenêtre d'affichage
    QTransform inverted = painter.transform().inverted();

    QPointF left = inverted.map(QPointF(0, 0));
    QPointF right = inverted.map(QPointF(width, 0));

    // Dessin des pistons.
    for (auto& piston : mState.pistons)
    {
        painter.setBrush(piston->color());
        painter.drawRect(QRectF(left.x(), piston->position().y, right.x() - left.x(), piston->epaisseur()));
    }

    // Dessin des populations.
    for (auto& boule : mState.boules)
    {
        painter.setBrush(boule->color());
        painter.drawEllipse(QRectF(boule->position().x - boule->rayon(), boule->position().y - boule->rayon(), 2 * boule->rayon(), 2 * boule->rayon()));
    }
}


// Effectue une collision.
bool Simulateur::performCollision(Collision& collision)
{
    collision.doCollision(mState);
    if (collision.isReal())
        ++mState.countChocs;
    return false;
}

// Met à jour le dessin.
bool Simulateur::performDrawEvent()
{
    auto& frames = mState.frames;
    if ((!frames.empty()) && frames.front().first.elapsed() >= 2000)
        this->emitStatusText(frames.front().first.elapsed(), frames.size(), mState.countChocs - frames.front().second, mState.countChocs);
    while ((!frames.empty()) && frames.front().first.elapsed() >= 2000)
        frames.pop_front();

    frames.push_back(std::make_pair(QTime(), mState.countChocs));
    frames.back().first.start();

    emit draw();
    QCoreApplication::processEvents();

    return true;
}

// Met à jour les valeurs des courbes.
bool Simulateur::performValueEvent()
{
    mGroupCourbes->push(mState);
    return true;
}

// Met à jour les courbes.
bool Simulateur::performCourbeEvent()
{
    mGroupCourbes->update();
    return true;
}

// Change la boule de population.
bool Simulateur::performBouleEvent(Boule* boule)
{
    boule->changePopulation(mState);
    return false;
}


// Ajoute un événement.
void Simulateur::addDrawEvent()
{
    mState.events.insert(
                std::make_pair(
                    mState.now + mState.stepDraw,
                    std::make_shared<DrawEvent>()
                ));
}

void Simulateur::addValueEvent()
{
    mState.events.insert(
                std::make_pair(
                    mState.now + mState.stepValues,
                    std::make_shared<ValueEvent>()
                ));
}

void Simulateur::addCourbeEvent()
{
    mState.events.insert(
                std::make_pair(
                    mState.now + mState.stepCourbes,
                    std::make_shared<CourbeEvent>()
                ));
}


// Change la fréquence d'affichage.
void Simulateur::setVitesse(int value)
{
    mState.stepDraw = std::pow(10, value / 250.0);
}

// Change la fréquence de sondage de valeurs.
void Simulateur::setValues(int value)
{
    mState.stepValues = std::pow(10, -value / 250.0);
}

// Change la fréquence d'affichage des courbes.
void Simulateur::setCourbes(int value)
{
    mState.stepCourbes = std::pow(10, -value / 250.0);
}


// Génère un texte pour la barre de statut (images par seconde, etc).
void Simulateur::emitStatusText(unsigned int msec, unsigned int frames, unsigned int chocs, unsigned int chocsTotal)
{
    mState.totalEtudes += mState.countEtudes.first;

    double fps = 1000.0 * frames / msec;
    unsigned int cps = 1000 * chocs / msec;/*
    unsigned int eps = 1000 * mCountEtudes.first / msec;
    //unsigned int epc = mCountEtudes / chocs;
    unsigned int epc = mTotalEtudes / chocsTotal;
    unsigned int spc = mCountEtudes.second / chocsTotal;
    unsigned int spe = 100 * (double)mCountEtudes.second / mTotalEtudes;
    //*/

    mState.countEtudes.first = 0;

    // Envoie le texte.
    emit statusText(QString::number((unsigned int)fps) + " frames per second ; " + QString::number(cps) + " collisions per second ; " + QString::number(chocsTotal) + " collisions in total");
}


// Met à jour les collisions en partant des mobiles concernés par la(les) dernière(s) effectuée(s).
void Simulateur::refreshCollisions()
{
    for (auto it = mState.toRefresh.begin() ; it != mState.toRefresh.end() ; ++it)
        (*it)->updateCollisions(mState);
    mState.toRefresh.clear();
}

// Met à jour les événements de dessin (supprime ceux qui viennent d'être effectués).
void Simulateur::refreshDrawings()
{
    for (auto it = mState.drawingsRefresh.begin() ; it != mState.drawingsRefresh.end() ; ++it)
    {
        (*it)->second->addEvent(*this);
        mState.events.erase(*it);
    }

    mState.drawingsRefresh.clear();
}


// Avance la simulation à un instant donné.
void Simulateur::avance(const Time& time)
{
    Time diff = time - mState.now;

    // Avance les populations de boules.
    for (auto& boule : mState.boules)
        boule->avance(diff, mState.config.gravity());

    // Avance les populations de pistons.
    for (auto& piston : mState.pistons)
        piston->avance(diff, mState.config.gravity());

    mState.now = time;
}


/*
// Vérifie que la simulation est cohérente (pas de chevauchements).
bool Simulateur::check() const
{
    for (int i = 0 ; i < mPopulations.size() ; ++i)
    {
        auto& boules = mPopulations[i].boules();
        for (auto it = boules.begin() ; it != boules.end() ; ++it)
        {
            const Boule& boule = **it;
            const Coord<double>& pos = boule.position();
            double rayon = boule.rayon();

            // Zone correcte pour chaque boule.
            if (pos.x < boule.area().x * mSizeArea || pos.x > (boule.area().x + 1) * mSizeArea || pos.y < boule.area().y * mSizeArea || pos.y > (boule.area().y + 1) * mSizeArea)
            {
                std::cout << "zone incorrecte : " << boule << " ; " << boule.area() << std::endl;
                return false;
            }

            // Chevauchements entre boules.
            for (int x = 0 ; x < mPopulations.size() && x <= i ; ++x)
            {
                for (auto iter = boules.begin() ; iter != boules.end() ; ++iter)
                {
                    if (it == iter)
                        break;

                    if (((*iter)->position() - pos).length() < (*iter)->rayon() + rayon)
                    {
                        std::cout << "collision (boules) : " << boule << " ; " << **iter << std::endl;
                        return false;
                    }
                }
            }

            // Chevauchements entre boules et pistons.
            for (auto& piston : mPistons)
            {
                if ((piston->position().y - pos.y) < rayon && (pos.y - piston->position().y) < rayon + piston->epaisseur())
                {
                    std::cout << "collision (piston) : " << boule << " ; " << *piston << std::endl;
                    return false;
                }
            }

            // Chevauchements entre boules et obstacles.
            for (auto& obstacle : mConfig.obstacles())
            {
                if (obstacle.sommets().intersect(pos, rayon) || obstacle.sommets().inside(pos))
                {
                    std::cout << "collision (obstacle) : " << boule << std::endl;
                    return false;
                }
            }

            // Chevauchements entre boules et le contour.
            if (mConfig.contour().sommets().intersect(pos, rayon) || !mConfig.contour().sommets().inside(pos))
            {
                std::cout << "collision (contour) : " << boule << std::endl;
                return false;
            }
        }
    }

    return true;
}
//*/
