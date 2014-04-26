/*
    Collisions - a real-time simulation program of colliding particles.
    Copyright (C) 2011 - 2014  G. Endignoux

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

#ifndef WIDGETCOURBE_HPP
#define WIDGETCOURBE_HPP

#include <QWidget>
#include "courbe.hpp"
#include "config_widgetcourbe.hpp"
#include "population.hpp"

// Widget pour afficher un ensemble de courbes.
class WidgetCourbe : public QWidget
{
public:
    // Construteur.
    WidgetCourbe(Time lifespan, const ConfigWidgetCourbe& config);

    // Calcule et ajoute des valeurs aux courbes.
    void push(Time time, const QList<Population>& populations, const QList<std::shared_ptr<Piston> >& pistons);
    // Redessine la QPixmap.
    void update();

    // Accesseurs.
    inline const QList<std::pair<Time, double> >& valeurs(unsigned int courbe) const;
    inline void setScroll(double scroll);
    inline void setLifespan(Time lifespan);
    inline void setFinished();

private:
    // Evénements de dessin.
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);

    // Image en cache.
    QPixmap mPixmap;

    // Données.
    Time mLifespan;
    double mScroll;
    bool mFinish;
    QList<Courbe> mCourbes;
    ConfigWidgetCourbe mConfig;
};

// Accesseurs.
inline const QList<std::pair<Time, double> >& WidgetCourbe::valeurs(unsigned int courbe) const
    {return mCourbes[courbe].valeurs();}
inline void WidgetCourbe::setScroll(double scroll)
    {mScroll = scroll;}
inline void WidgetCourbe::setLifespan(Time lifespan)
    {mLifespan = lifespan;}
inline void WidgetCourbe::setFinished()
    {mFinish = true;}

#endif // WIDGETCOURBE_HPP
