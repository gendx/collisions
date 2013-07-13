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

#ifndef EDIT_POLYGONE_HPP
#define EDIT_POLYGONE_HPP

#include "graph.hpp"
#include "double_delegate.hpp"

// Widget pour éditer la liste des points d'un polygone.
class EditPolygone : public QWidget
{
    Q_OBJECT

public:
    // Constructeur.
    EditPolygone();

    // Accesseurs.
    void setPolygone(Polygone polygone);
    void setPolygone();

signals:
    // Modification de l'utilisateur.
    void modified(Polygone polygone, QSet<unsigned int> selection);
    void statusText(QString);

public slots:
    // Ajoute un point au polygone.
    void point(Coord<double> point, Coord<double> normalized);
    // Point survolé par la souris.
    void hover(Coord<double> point, Coord<double> normalized);
    // Efface le point survolé.
    void noHover();

private slots:
    // Emet le signal "modified" selon les points et la sélection.
    void sendPolygone();
    // En cas de modification du modèle.
    void dataChanged(QStandardItem* item);

private:
    // Gestion de la touche suppression.
    void keyPressEvent(QKeyEvent* event);

    // Points du polygone.
    QList<Coord<double> > mPoints;
    // Interactions en cours avec l'utilisateur.
    Coord<double> mHoverPoint;
    bool mHasHover;
    bool mActive;

    QGridLayout* mLayout;
    QTableView* mTableView;
    QStandardItemModel* mModel;
    QCheckBox* mCheckGrid;
    DoubleDelegate* mDelegate;
};

#endif // EDIT_POLYGONE_HPP
