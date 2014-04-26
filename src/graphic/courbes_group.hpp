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

#ifndef COURBES_GROUP_HPP
#define COURBES_GROUP_HPP

#include <QVBoxLayout>
#include <QSplitter>
#include <QScrollBar>
#include "widgetcourbe.hpp"
#include "widgetprofil.hpp"

// Widget pour afficher un groupe de courbes.
class CourbesGroup : public QWidget
{
    Q_OBJECT

public:
    // Constructeur.
    CourbesGroup(Time lifespan);

    // Ajout/suppression de courbes.
    void clear();
    void addCourbe(const ConfigWidgetCourbe& courbe);
    void addProfil(const ConfigProfil& profil);

    // Ajout de valeurs aux courbes.
    void push(Time time, const QList<Population>& populations, const QList<std::shared_ptr<Piston> >& pistons);
    void update();

private slots:
    // Défilement horizontal.
    void scroll(int value);

private:
    // Molette de la souris.
    void wheelEvent(QWheelEvent* event);

    // Widgets.
    QVBoxLayout* mLayout;
    QSplitter* mSplitter;
    QScrollBar* mScrollBar;

    // Valeurs des courbes.
    QList<std::shared_ptr<WidgetCourbe> > mCourbes;
    QList<std::shared_ptr<WidgetProfil> > mProfils;
    Time mBegin;
    Time mEnd;
    Time mLifespan;
};

#endif // COURBES_GROUP_HPP
