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

#include "widgetcourbe.hpp"

#include <QPainter>
#include <QPaintEvent>

// Construteur.
WidgetCourbe::WidgetCourbe(Time lifespan, const ConfigWidgetCourbe& config) :
    QWidget(),
    mLifespan(lifespan),
    mScroll(0),
    mFinish(false),
    mCourbes(),
    mConfig(config)
{
    for (int i = 0 ; i < mConfig.mCourbes.size() ; ++i)
        mCourbes.push_back(Courbe(mConfig.mCourbes[i]));
    this->setMinimumSize(200, 50);
}


// Evénements de dessin.
void WidgetCourbe::paintEvent(QPaintEvent* event)
{
    if (mFinish)
        return;

    QPainter painter(this);
    QRect dirtyRect = event->rect();

    // Dessine la partie nécessaire.
    painter.drawPixmap(dirtyRect, mPixmap, dirtyRect);
}

void WidgetCourbe::resizeEvent(QResizeEvent*/* event*/)
{
    mPixmap = QPixmap(this->size());
    this->update();
}


// Calcule et ajoute des valeurs aux courbes.
void WidgetCourbe::push(Time time, const QList<Population>& populations, const QList<std::shared_ptr<Piston> >& pistons)
{
    for (int i = 0 ; i < mConfig.mCourbes.size() ; ++i)
        mCourbes[i].push(time, mConfig.mType, mConfig.mMean, populations, pistons);
}

// Redessine la QPixmap.
void WidgetCourbe::update()
{
    if (mFinish)
        return;

    // Création de la QPixmap.
    mPixmap.fill();

    // Aucune courbe -> rien à dessiner.
    if (mCourbes.isEmpty())
    {
        QWidget::update();
        return;
    }

    // Détermination des bornes.
    Time maxTime = mCourbes.front().maxTime();
    double max = mCourbes.front().max();
    double min = mCourbes.front().min();

    for (int i = 1 ; i < mCourbes.size() ; ++i)
    {
        if (maxTime < mCourbes[i].maxTime())
            maxTime = mCourbes[i].maxTime();
        if (max < mCourbes[i].max())
            max = mCourbes[i].max();
        if (min > mCourbes[i].min())
            min = mCourbes[i].min();
    }

    if (max < 0)
        max = 0;
    if (min > 0)
        min = 0;

    // Mise à l'échelle.
    QPainter painter(&mPixmap);
    painter.scale(this->width(), this->height());

    // Dessin d'une ligne horizontale en 0 si nécessaire.
    if (max > 0 && min < 0)
    {
        QPainterPath path;

        path.moveTo(QPointF(0, 1 + min / (max - min)));
        path.lineTo(QPointF(1, 1 + min / (max - min)));

        painter.setRenderHint(QPainter::Antialiasing, false);
        painter.strokePath(path, QColor(0xA0, 0xA0, 0xA0));
    }

    painter.setRenderHint(QPainter::Antialiasing);
    double start = maxTime.time() - mLifespan.time() * (1 + mScroll);

    // Tracé en reliant les points par des segments.
    for (int i = 0 ; i < mCourbes.size() ; ++i)
    {
        QPainterPath path;

        for (int j = 0 ; j < mCourbes[i].valeurs().size() ; ++j)
        {
            const std::pair<Time, double>& valeur = mCourbes[i].valeurs()[j];
            QPointF point((valeur.first.time() - start) / mLifespan.time(), (1 - (valeur.second - min) / (max - min)));

            if (j == 0)
                path.moveTo(point);
            else
                path.lineTo(point);
        }

        painter.strokePath(path, mCourbes[i].color());
    }

    // Met à jour le widget.
    QWidget::update();
}
