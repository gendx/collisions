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

#ifndef GRAPH_ZONE_HPP
#define GRAPH_ZONE_HPP

#include <QtGui>
#include "coord.hpp"

class Document;
class Graph;

// Widget bas niveau pour gérer le dessin et la navigation dans un repère.
class GraphZone : public QWidget
{
    Q_OBJECT

    friend class Graph;

public:
    // Constructeur.
    GraphZone(Graph* graph);

signals:
    // Signaux.
    void draw();
    void fullDraw();
    void point(Coord<double>);
    void hover(Coord<double>);
    void noHover();

private slots:
    // Centre la vue sur l'origine du repère.
    void center();

private:
    // Transforme un point de l'écran en point du repère.
    Coord<double> mapPoint(QPoint pos) const;

    // Gestion d'événements.
    void wheelEvent(QWheelEvent* event);
    void paintEvent(QPaintEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);

    // Widgets.
    Graph* mGraph;
    QAction* mActionCenter;

    // Pixmap en cache.
    QPixmap mPixmap;
    // Informations sur la souris et le clavier.
    QPoint mMousePos;
    bool mMousePress;
    bool mCtrl;
    double mZoom;
    Coord<double> mTranslate;
};

#endif // GRAPH_ZONE_HPP
