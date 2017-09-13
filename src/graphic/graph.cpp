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

#include "graph.hpp"

#include "document.hpp"

// Constructeur.
Graph::Graph(const Configuration& config) :
    mLayout(new QGridLayout(this)),
    mZone(new GraphZone(this)),
    mLabelZoom(new QLabel("zoom :")),
    mSliderZoom(new QSlider(Qt::Horizontal)),
    mConfig(config),
    mEditMode(false)
{
    mSliderZoom->setRange(-500, 500);

    // Création de l'interface graphique.
    mLayout->setMargin(0);
    mLayout->addWidget(mZone, 0, 0, 1, 2);
    mLayout->addWidget(mLabelZoom, 1, 0);
    mLayout->addWidget(mSliderZoom, 1, 1);

    // Connexion des signaux et slots.
    QObject::connect(mSliderZoom, SIGNAL(valueChanged(int)), this, SLOT(setZoom(int)));
    QObject::connect(mZone, SIGNAL(draw()), this, SIGNAL(draw()));
    QObject::connect(mZone, SIGNAL(fullDraw()), this, SIGNAL(fullDraw()));
    QObject::connect(mZone, SIGNAL(point(Coord<double>)), this, SLOT(pointEvent(Coord<double>)));
    QObject::connect(mZone, SIGNAL(hover(Coord<double>)), this, SLOT(hoverEvent(Coord<double>)));
    QObject::connect(mZone, SIGNAL(noHover()), this, SIGNAL(noHover()));

    mSliderZoom->setValue(0);
}


// Passe au mode édition.
void Graph::setEditMode()
{
    mEditMode = true;
    emit fullDraw();
}

// Passe au mode simulation.
void Graph::setSimulMode()
{
    mEditMode = false;
    emit fullDraw();
}


// Changement de zoom.
void Graph::setZoom(int value)
{
    mZone->mZoom = std::pow(10, value / 250.0);
    emit fullDraw();
}

// Définit la liste de polygones associée.
void Graph::setPolygones(QList<DrawPolygone> polygones)
{
    mPolygones = polygones;
    emit fullDraw();
}

// Trouve le point du quadrillage le plus proche.
Coord<double> Graph::normalize(Coord<double> pt) const
{
    int logZoom = std::floor(1.8 - std::log10(mZone->mZoom));
    double space = std::pow(10, logZoom);

    return Coord<double>(std::round(pt.x / space) * space, std::round(pt.y / space) * space);
}

// Evénements de souris sur un point du graphe.
void Graph::pointEvent(Coord<double> pt)
{
    emit point(pt, this->normalize(pt));
}

void Graph::hoverEvent(Coord<double> pt)
{
    emit hover(pt, this->normalize(pt));
}

// Mise à l'échelle du QPainter.
void Graph::myInitPainter(QPainter& painter) const
{
    painter.translate(mZone->width() / 2, mZone->height() / 2);
    painter.scale(mZone->mZoom, mZone->mZoom);
    painter.translate(mZone->mTranslate.x, mZone->mTranslate.y);
}

// Redessine la QPixmap de fond.
void Graph::reinitBackground(bool force)
{
    if (mZone->size() == mOldSize && !force)
        return;
    mOldSize = mZone->size();

    mBackground = QPixmap(mZone->size());

    QPainter painter(&mBackground);
    this->myInitPainter(painter);
    painter.setRenderHint(QPainter::Antialiasing);

    // En mode édition.
    if (mEditMode)
    {
        mBackground.fill(Qt::white);

        // Trace les polygones.
        for (auto& polygone : mPolygones)
            polygone.draw(painter);
    }
    // En mode simulation.
    else
    {
        mBackground.fill(mConfig.contour().color());

        // Trace le contour.
        painter.setBrush(Qt::white);
        painter.setPen(mConfig.contour().color());
        painter.drawPolygon(mConfig.contour().sommets().toPolygon());

        // Trace les obstacles.
        for (auto& obstacle : mConfig.obstacles())
        {
            painter.setBrush(obstacle.color());
            painter.setPen(obstacle.color());
            painter.drawPolygon(obstacle.sommets().toPolygon());
        }
    }

    // Trace le quadrillage.
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(QPen());

    this->drawAxes(painter);
}


// Dessine le quadrillage.
void Graph::drawAxes(QPainter& painter) const
{
    QTransform inverted = painter.transform().inverted();

    QPointF leftTop = inverted.map(QPointF(0, 0));
    QPointF rightBottom = inverted.map(QPointF(this->size().width(), this->size().height()));

    int logZoom = std::floor(1.8 - std::log10(mZone->mZoom));
    double remain = 2 - std::log10(mZone->mZoom) - logZoom;

    // Détermine les nuances de gris à utiliser suivant le zoom.
    int _grey = 0x20 + 0x80 * (1 - remain);
    int _black = 0x17F - _grey;
    if (_black > 0xFF)
        _black = 0xFF;
    QColor grey(0x00, 0x00, 0x00, _grey);
    QColor black(0x00, 0x00, 0x00, _black);

    // Détermine les lignes minimum et maximum.
    double space = std::pow(10, logZoom);
    Coord<int> min = Coord<int>(std::floor(leftTop.x() / space), std::floor(leftTop.y() / space));
    Coord<int> max = Coord<int>(std::ceil(rightBottom.x() / space), std::ceil(rightBottom.y() / space));

    // Dessine les lignes de différentes couleurs.
    Graph::drawPartialAxes(painter, grey, min, max, space, 1);
    Graph::drawPartialAxes(painter, black, min, max, space, 10.0);
    Graph::drawPartialAxes(painter, Qt::black, min, max, space, 100.0, false);
}

// Dessine un maillage de lignes de même couleur.
void Graph::drawPartialAxes(QPainter& painter, const QColor& color, Coord<int> min, Coord<int> max, double space, double ratio, bool skipBig)
{
    painter.setPen(color);
    for (int i = std::floor(min.x / ratio) ; i <= std::ceil(max.x / ratio) ; ++i)
    {
        if (skipBig && i % 10 == 0)
            continue;
        painter.drawLine(QLineF(ratio * i * space, min.y * space,
                                ratio * i * space, max.y * space));
    }
    for (int i = std::floor(min.y / ratio) ; i <= std::ceil(max.y / ratio) ; ++i)
    {
        if (skipBig && i % 10 == 0)
            continue;
        painter.drawLine(QLineF(min.x * space, ratio * i * space,
                                max.x * space, ratio * i * space));
    }
}
