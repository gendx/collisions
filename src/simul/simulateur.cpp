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

#include "simulateur.hpp"

#include <QPainter>
#include <QCoreApplication>
#include "coord_io.tpl"

// Constructeur.
Simulateur::Simulateur(const Configuration& config) :
    mLayout(new QGridLayout(this)),
    mGroupCourbes(new CourbesGroup(500)),
    mLabelVitesse(new QLabel("vitesse :")),
    mSliderVitesse(new QSlider(Qt::Horizontal)),/*
    mLabelArea(new QLabel("area (20) :")),
    mSliderArea(new QSlider(Qt::Horizontal)),//*/
    mLabelValues(new QLabel(QString::fromUtf8("valeurs :"))),
    mSliderValues(new QSlider(Qt::Horizontal)),
    mLabelCourbes(new QLabel(QString::fromUtf8("courbes :"))),
    mSliderCourbes(new QSlider(Qt::Horizontal)),
    mSizeArea(),
    mCountChocs(0),
    mCountEtudes(0, 0),
    mTotalEtudes(0),
    mConfig(config)
{
    // Création de l'interface graphique.
    mSliderVitesse->setRange(-1000, 250);
    //mSliderArea->setRange(20, 200);
    mSliderValues->setRange(-500, 500);
    mSliderCourbes->setRange(-750, 250);

    mLayout->setMargin(0);
    mLayout->addWidget(mGroupCourbes, 0, 0, 1, 2);
    mLayout->addWidget(mLabelVitesse, 1, 0);
    mLayout->addWidget(mSliderVitesse, 1, 1);/*
    mLayout->addWidget(mLabelArea, 2, 0);
    mLayout->addWidget(mSliderArea, 2, 1);//*/
    mLayout->addWidget(mLabelValues, 2, 0);
    mLayout->addWidget(mSliderValues, 2, 1);
    mLayout->addWidget(mLabelCourbes, 3, 0);
    mLayout->addWidget(mSliderCourbes, 3, 1);

    // Connexion des signaux et slots.
    QObject::connect(mSliderVitesse, SIGNAL(valueChanged(int)), this, SLOT(setVitesse(int)));
    //QObject::connect(mSliderArea, SIGNAL(valueChanged(int)), this, SLOT(setArea(int)));
    QObject::connect(mSliderValues, SIGNAL(valueChanged(int)), this, SLOT(setValues(int)));
    QObject::connect(mSliderCourbes, SIGNAL(valueChanged(int)), this, SLOT(setCourbes(int)));

    // Initialisation.
    mSliderVitesse->setValue(-500);
    //mSliderArea->setValue(20);
    mSliderValues->setValue(0);
    mSliderCourbes->setValue(-250);

    this->setValues(0);
}


// Redémarre la simulation.
void Simulateur::doRestart()
{
    // Destruction de la simulation précédente.
    mEvents.clear();
    mToRefresh.clear();
    mDrawingsRefresh.clear();
    mPopulations.clear();
    mPistons.clear();
    mMapMobiles.clear();
    mNow = 0;
    mCountChocs = 0;
    mCountEtudes.first = 0;
    mCountEtudes.second = 0;
    mTotalEtudes = 0;
    mFrames.clear();
    mGroupCourbes->clear();

    mSizeArea = mConfig.sizeArea();

    // Création des obstacles.
    this->addObstacles();

    // Création des pistons.
    for (int i = 0 ; i < mConfig.configPistons().size() ; ++i)
    {
        mPistons.push_back(std::make_shared<Piston>(mConfig.configPistons()[i], mSizeArea, mMapMobiles));
        mPistons.back()->updateCollisions(mEvents, mMapMobiles, mNow, mSizeArea, mConfig.gravity(), mCountEtudes);
    }

    // Création des populations.
    for (int i = 0 ; i < mConfig.configPops().size() ; ++i)
    {
        mPopulations.push_back(Population(mConfig.configPops()[i]));
        mPopulations.back().create(mConfig, mPopulations, i, mMapMobiles, mPistons, mEvents, mNow, mSizeArea);
    }

    // Création des courbes.
    for (int i = 0 ; i < mConfig.configFcourbes().size() ; ++i)
        mGroupCourbes->addCourbe(mConfig.configFcourbes()[i]);

    // Création des profils.
    for (int i = 0 ; i < mConfig.configProfils().size() ; ++i)
        mGroupCourbes->addProfil(mConfig.configProfils()[i]);

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
    Time timeEvent = mEvents.begin()->first;
    this->avance(timeEvent);

    //*
    bool isDraw = false;
    for (auto it = mEvents.begin() ; it != mEvents.end() && it->first == mNow ; ++it)
        if (it->second->perform(*this, isDraw))
            mDrawingsRefresh.push_back(it);
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
        //QMessageBox::warning(this, "attention", QString::fromUtf8("Plusieurs collisions simultanées !"));
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
    for (int i = 0 ; i < mPistons.size() ; ++i)
    {
        const Piston* piston = mPistons[i].get();
        painter.setBrush(piston->color());
        painter.drawRect(QRectF(left.x(), piston->position().y, right.x() - left.x(), piston->epaisseur()));
    }

    // Dessin des populations.
    for (int i = 0 ; i < mPopulations.size() ; ++i)
    {
        for (auto it = mPopulations[i].boules().begin() ; it != mPopulations[i].boules().end() ; ++it)
        {
            const Boule* boule = it->get();

            painter.setBrush(boule->color());
            painter.drawEllipse(QRectF(boule->position().x - boule->rayon(), boule->position().y - boule->rayon(), 2 * boule->rayon(), 2 * boule->rayon()));
        }
    }
}


// Effectue une collision.
bool Simulateur::performCollision(Collision& collision)
{
    collision.doCollision(mNow, mSizeArea, mToRefresh, mEvents, mConfig.configMutations(), mConfig.configReactions(), mPopulations, mMapMobiles, mCountEtudes);
    if (collision.isReal())
        ++mCountChocs;
    return false;
}

// Met à jour le dessin.
bool Simulateur::performDrawEvent()
{
    if ((!mFrames.empty()) && mFrames.front().first.elapsed() >= 2000)
        this->emitStatusText(mFrames.front().first.elapsed(), mFrames.size(), mCountChocs - mFrames.front().second, mCountChocs);
    while ((!mFrames.empty()) && mFrames.front().first.elapsed() >= 2000)
        mFrames.pop_front();

    mFrames.append(std::make_pair(QTime(), mCountChocs));
    mFrames.back().first.start();

    emit draw();
    QCoreApplication::processEvents();

    return true;
}

// Met à jour les valeurs des courbes.
bool Simulateur::performValueEvent()
{
    mGroupCourbes->push(mNow, mPopulations, mPistons);
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
    boule->changePopulation(mNow, mPopulations, mEvents, mConfig.configMutations());
    return false;
}


// Ajoute un événement.
void Simulateur::addDrawEvent()
{
    mEvents.insert(std::make_pair(mNow + mStepDraw, std::make_shared<DrawEvent>()));
}

void Simulateur::addValueEvent()
{
    mEvents.insert(std::make_pair(mNow + mStepValues, std::make_shared<ValueEvent>()));
}

void Simulateur::addCourbeEvent()
{
    mEvents.insert(std::make_pair(mNow + mStepCourbes, std::make_shared<CourbeEvent>()));
}


// Change la fréquence d'affichage.
void Simulateur::setVitesse(int value)
{
    mStepDraw = pow(10, value / 250.0);
}

/*
// Change la taille des zones.
void Simulateur::setArea(int)
{
    if (mPlaying)
        mWaitArea = true;
    else
        this->rehashArea();
}//*/

// Change la fréquence de sondage de valeurs.
void Simulateur::setValues(int value)
{
    mStepValues = pow(10, -value / 250.0);
}

// Change la fréquence d'affichage des courbes.
void Simulateur::setCourbes(int value)
{
    mStepCourbes = pow(10, -value / 250.0);
}


// Ajoute les obstacles à la simulation.
void Simulateur::addObstacles()
{
    // ajout des sommets
    for (int i = 0 ; i < mConfig.obstacles().size() ; ++i)
        this->addObstacle(mConfig.obstacles()[i].sommets());
    this->addObstacle(mConfig.contour().sommets());
}

// Ajoute un obstacle à la simulation.
void Simulateur::addObstacle(const Polygone& sommets)
{
    for (unsigned int j = 0 ; j < sommets.size() ; ++j)
    {
        const Coord<double>& point = sommets.point(j);
        mMapMobiles[floor(point.y / mSizeArea)].sommets().insert(floor(point.x / mSizeArea), point);
        this->addSegment(sommets.segment(j));
    }
}

// Ajoute un segment à la simulation.
void Simulateur::addSegment(const Segment& segment)
{
    // Extrémités du segment.
    Coord<int> point1 = segment.point1(mSizeArea);
    Coord<int> point2 = segment.point2(mSizeArea);

    // Rectangle contenant le segment.
    Coord<int> min = point1.min(point2);
    Coord<int> max = point1.max(point2);

    // Ajout aux zones intersectées.
    for (int i = min.x ; i < max.x ; ++i)
    {
        int y = floor(segment.yAtX(i * mSizeArea) / mSizeArea);
        mMapMobiles[y].segments()[i].insert(segment);
        mMapMobiles[y].segments()[i + 1].insert(segment);
    }

    for (int j = min.y ; j < max.y ; ++j)
    {
        int x = floor(segment.xAtY(j * mSizeArea) / mSizeArea);
        mMapMobiles[j].segments()[x].insert(segment);
        mMapMobiles[j + 1].segments()[x].insert(segment);
    }
}


// Génère un texte pour la barre de statut (images par seconde, etc).
void Simulateur::emitStatusText(unsigned int msec, unsigned int frames, unsigned int chocs, unsigned int chocsTotal)
{
    mTotalEtudes += mCountEtudes.first;

    double fps = 1000.0 * frames / msec;
    unsigned int cps = 1000 * chocs / msec;/*
    unsigned int eps = 1000 * mCountEtudes.first / msec;
    //unsigned int epc = mCountEtudes / chocs;
    unsigned int epc = mTotalEtudes / chocsTotal;
    unsigned int spc = mCountEtudes.second / chocsTotal;
    unsigned int spe = 100 * (double)mCountEtudes.second / mTotalEtudes;
    //*/

    mCountEtudes.first = 0;

    // Envoie le texte.
    emit statusText(QString::number(fps, 'g', 2) + " images par seconde ; " + QString::number(cps) + " chocs par seconde ; " + QString::number(chocsTotal) + " chocs au total");
}


// Met à jour les collisions en partant des mobiles concernés par la(les) dernière(s) effectuée(s).
void Simulateur::refreshCollisions()
{
    for (auto it = mToRefresh.begin() ; it != mToRefresh.end() ; ++it)
        (*it)->updateCollisions(mEvents, mMapMobiles, mNow, mSizeArea, mConfig.gravity(), mCountEtudes);
    mToRefresh.clear();
}

// Met à jour les événements de dessin (supprime ceux qui viennent d'être effectués).
void Simulateur::refreshDrawings()
{
    for (auto it = mDrawingsRefresh.begin() ; it != mDrawingsRefresh.end() ; ++it)
    {
        (*it)->second->addEvent(*this);
        mEvents.erase(*it);
    }

    mDrawingsRefresh.clear();
}


// Avance la simulation à un instant donné.
void Simulateur::avance(const Time& time)
{
    Time diff = time - mNow;

    // Avance les populations de boules.
    for (int i = 0 ; i < mPopulations.size() ; ++i)
        for (auto it = mPopulations[i].boules().begin() ; it != mPopulations[i].boules().end() ; ++it)
            (*it)->avance(diff, mConfig.gravity());

    // Avance les populations de pistons.
    for (int i = 0 ; i < mPistons.size() ; ++i)
        mPistons[i]->avance(diff, mConfig.gravity());

    mNow = time;
}


/*
// Change la taille des zones (non opérationel).
void Simulateur::rehashArea()
{
    mSizeArea = mSliderArea->value();
    mLabelArea->setText(QString("area (") + QString::number(mSliderArea->value()) + ") :");

    mMapMobiles.clear();

    for (unsigned int i = 0 ; i < mPopulations.size() ; ++i)
        for (auto it = mPopulations[i].boules().begin() ; it != mPopulations[i].boules().end() ; ++it)
            (*it)->setArea(mSizeArea, mToRefresh, mMapMobiles);

    this->refreshCollisions();
}

// Vérifie que la simulation est cohérente (pas de chevauchements).
bool Simulateur::check() const
{
    for (int i = 0 ; i < mPopulations.size() ; ++i)
    {
        for (auto it = mPopulations[i].boules().begin() ; it != mPopulations[i].boules().end() ; ++it)
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
                for (auto iter = mPopulations[i].boules().begin() ; iter != mPopulations[i].boules().end() ; ++iter)
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
            for (int k = 0 ; k < mPistons.size() ; ++k)
            {
                if ((mPistons[k]->position().y - pos.y) < rayon && (pos.y - mPistons[k]->position().y) < rayon + mPistons[k]->epaisseur())
                {
                    std::cout << "collision (piston) : " << boule << " ; " << *mPistons[k] << std::endl;
                    return false;
                }
            }

            // Chevauchements entre boules et obstacles.
            for (int k = 0 ; k < mConfig.obstacles().size() ; ++k)
            {
                if (mConfig.obstacles()[k].sommets().intersect(pos, rayon) || mConfig.obstacles()[k].sommets().inside(pos))
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
