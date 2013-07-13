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

#include "draw_polygone.hpp"

// Dessine le polygone sur le QPainter.
void DrawPolygone::draw(QPainter& painter)
{
    // Définit les paramètres du crayon.
    QPen pen(mColor);
    pen.setWidth(10);
    pen.setCapStyle(Qt::SquareCap);
    pen.setCosmetic(true);

    QPen penSelect(pen);
    penSelect.setWidth(16);
    penSelect.setColor(Qt::black);

    painter.setBrush(Qt::NoBrush);

    // Dessine les sommets.
    for (unsigned int i = 0 ; i < mPolygone.size() ; ++i)
    {
        Coord<double> point = mPolygone.point(i);
        if (mSelectedPts.contains(i))
        {
            painter.setPen(penSelect);
            painter.drawPoint(QPointF(point.x, point.y));
        }

        painter.setPen(pen);
        painter.drawPoint(QPointF(point.x, point.y));
    }

    // Dessine le polygone.
    pen.setDashPattern(mDashPattern);
    pen.setWidth(4);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    painter.setPen(pen);
    painter.drawPolygon(mPolygone.toPolygon());
}
