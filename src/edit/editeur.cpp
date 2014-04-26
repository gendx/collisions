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

#include "editeur.hpp"

#include <QPainter>

// Constructeur.
Editeur::Editeur(Graph* graph) :
    mGraph(graph),
    mLayout(new QGridLayout(this)),
    mCombo(new QComboBox),
    mStackedWidget(new QStackedWidget),
    mEditPolygone(new EditPolygone),
    mEditObstacles(new EditObstacles(mEditPolygone)),
    mEditPopulations(new EditPopulations(mEditPolygone)),
    mEditPistons(new EditPistons(mEditPolygone)),
    mEditTransformations(new EditTransformations),
    mEditCourbes(new EditCourbes),
    mEditDivers(new EditDivers)
{
    // Création de l'interface graphique.
    mCombo->addItem("obstacles");
    mCombo->addItem("populations");
    mCombo->addItem("pistons");
    mCombo->addItem("transformations");
    mCombo->addItem("courbes");
    mCombo->addItem("divers");

    mStackedWidget->addWidget(mEditObstacles);
    mStackedWidget->addWidget(mEditPopulations);
    mStackedWidget->addWidget(mEditPistons);
    mStackedWidget->addWidget(mEditTransformations);
    mStackedWidget->addWidget(mEditCourbes);
    mStackedWidget->addWidget(mEditDivers);

    mLayout->setMargin(0);
    mLayout->addWidget(mCombo, 0, 0);
    mLayout->addWidget(mStackedWidget, 1, 0);
    mLayout->addWidget(mEditPolygone, 2, 0);

    // Connexion des signaux et slots.
    QObject::connect(mCombo, SIGNAL(activated(int)), this, SLOT(activate(int)));
    QObject::connect(mEditPolygone, SIGNAL(modified(Polygone, QSet<unsigned int>)),
                     this, SLOT(setPolygone(Polygone, QSet<unsigned int>)));
    QObject::connect(mEditPolygone, SIGNAL(statusText(QString)), this, SIGNAL(statusText(QString)));

    // activate 0
    QObject::connect(mEditObstacles, SIGNAL(polygones(QList<Polygone>)), this, SLOT(setPolygones(QList<Polygone>)));
}


// Import de configuration.
void Editeur::setConfig(const Configuration& config)
{
    mEditObstacles->setContour(config.contour());
    mEditObstacles->setObstacles(config.obstacles());
    mEditPopulations->setPopulations(config.configPops());
    mEditPistons->setPistons(config.configPistons());
    mEditTransformations->setMutations(config.configMutations());
    mEditTransformations->setReactions(config.configReactions());
    mEditDivers->setGravity(config.gravity());
}

// Export de configuration.
Configuration Editeur::config() const
{
    Configuration result;

    result.setContour(mEditObstacles->contour());
    result.setObstacles(mEditObstacles->obstacles());
    result.configPops() = mEditPopulations->config();
    result.configPistons() = mEditPistons->config();
    result.configMutations() = mEditTransformations->configMutations();
    result.configReactions() = mEditTransformations->configReactions();
    result.setGravity(mEditDivers->gravity());

    return result;
}


// Dessine le polygone en cours sur le QPainter.
void Editeur::draw(QPainter& painter)
{
    painter.setRenderHint(QPainter::Antialiasing);
    mDrawPolygone.draw(painter);
}


// Connexion de signaux et slots à l'affichage.
void Editeur::showEvent(QShowEvent*)
{
    QObject::connect(mGraph, SIGNAL(point(Coord<double>, Coord<double>)), mEditPolygone, SLOT(point(Coord<double>, Coord<double>)));
    QObject::connect(mGraph, SIGNAL(hover(Coord<double>, Coord<double>)), mEditPolygone, SLOT(hover(Coord<double>, Coord<double>)));
    QObject::connect(mGraph, SIGNAL(noHover()), mEditPolygone, SLOT(noHover()));
}

// Déconnexion de signaux et slots en fin d'affichage.
void Editeur::hideEvent(QHideEvent*)
{
    QObject::disconnect(mGraph, SIGNAL(point(Coord<double>, Coord<double>)), mEditPolygone, SLOT(point(Coord<double>, Coord<double>)));
    QObject::disconnect(mGraph, SIGNAL(hover(Coord<double>, Coord<double>)), mEditPolygone, SLOT(hover(Coord<double>, Coord<double>)));
    QObject::disconnect(mGraph, SIGNAL(noHover()), mEditPolygone, SLOT(noHover()));
}


// Active un widget d'édition dans le QStackedWidget.
void Editeur::activate(int index)
{
    // Deconnexion de signaux et slots avec le widget courant.
    if (mStackedWidget->currentIndex() == 0)
        QObject::disconnect(mEditObstacles, SIGNAL(polygones(QList<Polygone>)), this, SLOT(setPolygones(QList<Polygone>)));
    else if (mStackedWidget->currentIndex() == 1)
        QObject::disconnect(mEditPopulations, SIGNAL(polygones(QList<Polygone>)), this, SLOT(setPolygones(QList<Polygone>)));

    // Changement du widget courant.
    mStackedWidget->setCurrentIndex(index);

    // Connexion de signaux et slots avec le nouveau widget courant.
    if (index == 0)
    {
        QObject::connect(mEditObstacles, SIGNAL(polygones(QList<Polygone>)), this, SLOT(setPolygones(QList<Polygone>)));
        mEditObstacles->setPolygone();
    }
    else if (index == 1)
    {
        QObject::connect(mEditPopulations, SIGNAL(polygones(QList<Polygone>)), this, SLOT(setPolygones(QList<Polygone>)));
        mEditPopulations->setPolygone();
    }
    else
    {
        this->setPolygones(QList<Polygone>());
        mEditPolygone->setPolygone();
    }
}


// Définit le polygone en cours d'édition.
void Editeur::setPolygone(Polygone polygone, QSet<unsigned int> selection)
{
    mDrawPolygone = DrawPolygone(polygone, /*QColor(0xFF609010), */QColor(0xFFFF8000), QVector<qreal>() << 3 << 3, selection);
    emit draw();
}

// Définit les polygones en cours d'édition.
void Editeur::setPolygones(QList<Polygone> polygones)
{
    QList<DrawPolygone> result;

    for (int i = 0 ; i < polygones.size() ; ++i)
        result.append(DrawPolygone(polygones[i], /*QColor(0xFF609010), */QColor(0xFFC0C0E0), QVector<qreal>() << 3 << 3, QSet<unsigned int>()));

    mGraph->setPolygones(result);
}
