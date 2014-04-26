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

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include "graph_zone.hpp"
#include "configuration.hpp"
#include "draw_polygone.hpp"

// Widget sur lequel sont dessinés les particules et les obstacles.
class Graph : public QWidget
{
    Q_OBJECT

    friend class GraphZone;

public:
    // Constructeur.
    Graph(const Configuration& config);

    // Accesseurs.
    inline QPixmap getBackground() const;
    inline void setPixmap(QPixmap pixmap);
    void setPolygones(QList<DrawPolygone> polygones);

    // Fonctions de dessin.
    void initPainter(QPainter& painter) const;
    void reinitBackground(bool force/* = true*/);

    // Change le mode de travail.
    void setEditMode();
    void setSimulMode();

signals:
    // Signaux.
    void draw();
    void fullDraw();
    void point(Coord<double>, Coord<double>);
    void hover(Coord<double>, Coord<double>);
    void noHover();

public slots:
    // Changement de zoom.
    void setZoom(int value);

private slots:
    // Evénements de souris sur un point du graphe.
    void pointEvent(Coord<double> pt);
    void hoverEvent(Coord<double> pt);

private:
    // Dessin du quadrillage.
    void drawAxes(QPainter& inverted) const;
    static void drawPartialAxes(QPainter& painter, const QColor& color, Coord<int> min, Coord<int> max, double space, double ratio, bool skipBig = true);
    // Trouve le point du quadrillage le plus proche.
    Coord<double> normalize(Coord<double> pt) const;

    // Widgets.
    QGridLayout* mLayout;
    GraphZone* mZone;
    QLabel* mLabelZoom;
    QSlider* mSliderZoom;

    // Données.
    QPixmap mBackground;
    QSize mOldSize;
    const Configuration& mConfig;
    QList<DrawPolygone> mPolygones;
    bool mEditMode;
};


// Renvoie l'image d'arrière plan.
inline QPixmap Graph::getBackground() const
    {return mBackground;}

inline void Graph::setPixmap(QPixmap pixmap)
    {mZone->mPixmap = pixmap;}

#endif // GRAPH_HPP
