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

#include "widgetprofil.hpp"

#include <QPainter>
#include <QPaintEvent>

// Construteur.
WidgetProfil::WidgetProfil(Time lifespan, const ConfigProfil& config) :
    QWidget(),
    mLifespan(lifespan),
    mScroll(0),
    mFinish(false),
    mProfil(config)
{
    this->setMinimumSize(200, 50);
}


// Evénements de dessin.
void WidgetProfil::paintEvent(QPaintEvent* event)
{
    if (mFinish)
        return;

    QPainter painter(this);
    QRect dirtyRect = event->rect();

    painter.drawPixmap(dirtyRect, mPixmap, dirtyRect);
}

void WidgetProfil::resizeEvent(QResizeEvent*/* event*/)
{
    mPixmap = QPixmap(this->size());
    this->update();
}


// Redessine la QPixmap.
void WidgetProfil::update()
{
    if (mFinish)
        return;

    // Création et mise à l'échelle.
    mPixmap.fill();

    QPainter painter(&mPixmap);
    painter.scale(this->width(), this->height());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    // Détermination des bornes
    Time maxTime = mProfil.maxTime();
    Time minTime = mProfil.minTime();
    double max = mProfil.max();
    double start = maxTime.time() - mLifespan.time() * (1 + mScroll);
    int minSlice = mProfil.minSlice();
    int maxSlice = mProfil.maxSlice();
    Coord<double> unit(1.0 / mLifespan.time(), 1.0 / (maxSlice + 1 - minSlice));


    // Tracé par tranches horizontales
    for (int j = minSlice ; j <= maxSlice ; ++j)
    {
        // Dégradé de couleurs.
        QLinearGradient gradient(QPointF((minTime.time() - start) * unit.x, 0), QPointF((maxTime.time() - start) * unit.x, 0));

        // Couleur point par point.
        for (int i = 0 ; i < mProfil.valeurs().size() ; ++i)
        {
            Time time = mProfil.valeurs()[i].first;

            QMap<int, double>::const_iterator found = mProfil.valeurs()[i].second.find(j);

            // Choix de la couleur.
            QColor color = Qt::white;
            if (found != mProfil.valeurs()[i].second.end())
                color = QColor::fromHsl((unsigned int)(560 - 260 * found.value() / max) % 360, 255, 128);
            gradient.setColorAt((time.time() - minTime.time()) / (maxTime.time() - minTime.time()), color);
        }

        // Trace un rectangle horizontal pour la tranche.
        painter.setBrush(QBrush(gradient));
        QRectF rect((minTime.time() - start) * unit.x, (j - minSlice) * unit.y, (maxTime.time() - minTime.time()) * unit.x, unit.y);

        painter.drawRect(rect);
    }

    // Met à jour le widget.
    QWidget::update();
}
