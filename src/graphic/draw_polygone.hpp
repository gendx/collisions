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

#ifndef DRAW_POLYGONE_HPP
#define DRAW_POLYGONE_HPP

#include <QColor>
#include <QSet>
#include <QPainter>
#include "polygone.hpp"

// Classe contenant les données d'un polygone à dessiner.
class DrawPolygone
{
public:
    // Constructeurs.
    inline DrawPolygone();
    inline DrawPolygone(const Polygone& polygone, QColor color, const QVector<qreal>& dashPattern, const QSet<unsigned int>& selectedPts);

    // Dessine le polygone sur le QPainter.
    void draw(QPainter& painter);

private:
    // Données.
    Polygone mPolygone;
    QColor mColor;
    QVector<qreal> mDashPattern;
    QSet<unsigned int> mSelectedPts;
};


// Constructeurs.
inline DrawPolygone::DrawPolygone() :
    mPolygone(), mColor(Qt::black), mDashPattern(), mSelectedPts() {}
inline DrawPolygone::DrawPolygone(const Polygone& polygone, QColor color, const QVector<qreal>& dashPattern, const QSet<unsigned int>& selectedPts) :
    mPolygone(polygone), mColor(color), mDashPattern(dashPattern), mSelectedPts(selectedPts) {}

#endif // DRAW_POLYGONE_HPP
