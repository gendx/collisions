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

#ifndef EDITEUR_HPP
#define EDITEUR_HPP

#include <QComboBox>
#include <QStackedWidget>
#include "edit_polygone.hpp"
#include "edit_obstacles.hpp"
#include "edit_populations.hpp"
#include "edit_courbes.hpp"
#include "edit_transformations.hpp"
#include "edit_pistons.hpp"
#include "edit_divers.hpp"

// Widget pour éditer une configuration.
class Editeur : public QWidget
{
    Q_OBJECT

public:
    // Constructeur.
    Editeur(Graph* graph);

    // Import/export de configuration.
    void setConfig(const Configuration& config);
    Configuration config() const;

    // Dessine le polygone en cours sur le QPainter.
    void draw(QPainter& painter);

signals:
    // Requiert de redessiner le Graph.
    void draw();
    void fullDraw();
    // Statut à afficher dans une QStatusBar.
    void statusText(QString);

private slots:
    // Active un widget d'édition dans le QStackedWidget.
    void activate(int index);
    // Définit le polygone en cours d'édition.
    void setPolygone(Polygone polygone, QSet<unsigned int> selection);
    void setPolygones(QList<Polygone> polygones);

private:
    // Gestion des événements.
    void showEvent(QShowEvent*);
    void hideEvent(QHideEvent*);

    // Graph du Document associé.
    Graph* mGraph;

    // Widgets et layout.
    QGridLayout* mLayout;
    QComboBox* mCombo;
    QStackedWidget* mStackedWidget;
    EditPolygone* mEditPolygone;

    // Widgets d'édition des différentes parties de la configuration.
    EditObstacles* mEditObstacles;
    EditPopulations* mEditPopulations;
    EditPistons* mEditPistons;
    EditTransformations* mEditTransformations;
    EditCourbes* mEditCourbes;
    EditDivers* mEditDivers;

    // Polygone en cours d'édition.
    DrawPolygone mDrawPolygone;

    // Configuration associée.
    Configuration mConfig;
};

#endif // EDITEUR_HPP
