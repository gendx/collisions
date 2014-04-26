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

#ifndef EDIT_OBSTACLES_HPP
#define EDIT_OBSTACLES_HPP

#include <QListView>
#include "edit_withpolygone.hpp"

// Widget pour éditer les obstacles.
class EditObstacles : public EditWithPolygone
{
    Q_OBJECT

public:
    // Constructeur.
    EditObstacles(EditPolygone* editPolygone);

    // Accesseurs.
    void setContour(Obstacle contour);
    void setObstacles(QList<Obstacle> obstacles);

    Obstacle contour() const;
    QList<Obstacle> obstacles() const;

public slots:
    // Met à jour le polygone selon la sélection.
    void selectPolygone();

private slots:
    // Ajout d'un obstacle.
    void add();
    // Supprime l'obstacle sélectionné.
    void remove();

private:
    // Ajout d'un obtacle à la liste.
    void addCore();
    // Active la synchronisation du polygone avec la sélection.
    void connectSelection(bool connect);

    QListView* mListView;
};

#endif // EDIT_OBSTACLES_HPP
