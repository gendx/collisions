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

#include "graph_zone.hpp"

#include <QAction>
#include <QWheelEvent>
#include "graph.hpp"
#include "document.hpp"

// Constructeur.
GraphZone::GraphZone(Graph* graph) :
    mGraph(graph),
    mActionCenter(new QAction("&Center", this)),
    mMousePress(false),
    mCtrl(false),
    mZoom(1),
    mTranslate(0)
{
    this->setMouseTracking(true);
    this->setMinimumSize(200, 200);
    this->setCursor(Qt::OpenHandCursor);
    this->setFocusPolicy(Qt::StrongFocus);

    this->addAction(mActionCenter);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);

    // Connexion des signaux et slots.
    QObject::connect(mActionCenter, SIGNAL(triggered()), this, SLOT(center()));
}


// Centre la vue sur l'origine du repère.
void GraphZone::center()
{
    mTranslate.x = mTranslate.y = 0;
    emit fullDraw();
}

// Transforme un point de l'écran en point du repère.
Coord<double> GraphZone::mapPoint(QPoint pos) const
{
    // Matrice inverse de la transformation de dessin.
    QTransform inverted;
    inverted.translate(-mTranslate.x, -mTranslate.y);
    inverted.scale(1.0 / mZoom, 1.0 / mZoom);
    inverted.translate(-this->width() / 2, -this->height() / 2);

    QPointF realPt = inverted.map(QPointF(pos));
    return Coord<double>(realPt.x(), realPt.y());
}


// Molette de la souris -> changement du zoom.
void GraphZone::wheelEvent(QWheelEvent* event)
{
    mGraph->mSliderZoom->setValue(mGraph->mSliderZoom->value() + event->delta() / 16.0);
    event->accept();
}

void GraphZone::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();

    painter.drawPixmap(dirtyRect, mPixmap, dirtyRect);
}

// Clavier : touche control pour désactiver le déplacement de la vue par la souris.
void GraphZone::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control)
    {
        mCtrl = true;
        this->setCursor(Qt::ArrowCursor);
        event->accept();

        emit hover(this->mapPoint(mMousePos));
    }
}

void GraphZone::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control)
    {
        mCtrl = false;
        this->setCursor(Qt::OpenHandCursor);
        event->accept();

        emit noHover();
    }
}

// Vérifie les déplacements de souris.
void GraphZone::mouseMoveEvent(QMouseEvent* event)
{
    // Déplacement de la vue en cours.
    if (mMousePress)
    {
        QPointF diff = event->pos() - mMousePos;

        QTransform inverted;
        inverted.scale(1.0 / mZoom, 1.0 / mZoom);

        QPointF translate = inverted.map(diff);
        mTranslate.x += translate.x();
        mTranslate.y += translate.y();

        emit fullDraw();
    }
    // Touche control enclenchée.
    else if (mCtrl)
        emit hover(this->mapPoint(event->pos()));

    mMousePos = event->pos();
    event->accept();
}

void GraphZone::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
        return;

    // Touche control enclenchée.
    if (mCtrl)
        emit point(this->mapPoint(mMousePos));
    //
    else
    {
        mMousePress = true;
        emit draw();
        this->setCursor(Qt::ClosedHandCursor);
    }

    event->accept();
}

void GraphZone::mouseReleaseEvent(QMouseEvent* event)
{
    mMousePress = false;
    if (!mCtrl)
        this->setCursor(Qt::OpenHandCursor);
    event->accept();
}

// Entrée/sortie de la souris dans le widget.
void GraphZone::enterEvent(QEvent* event)
{
    this->setFocus();
    event->accept();
}

void GraphZone::leaveEvent(QEvent* event)
{
    mMousePos.setX(-1);
    mMousePos.setY(-1);
    mMousePress = false;
    emit noHover();

    event->accept();
}
