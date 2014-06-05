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

#ifndef WIDGETPROFIL_HPP
#define WIDGETPROFIL_HPP

#include <QWidget>
#include "profil.hpp"

// Widget pour afficher un profil.
class WidgetProfil : public QWidget
{
public:
    // Construteur.
    WidgetProfil(Time lifespan, const ConfigProfil& config);

    // Calcule et ajoute une tranche au profil.
    inline void push(State& state);
    // Redessine la QPixmap.
    void update();

    // Accesseurs.
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
    Profil mProfil;
};

// Calcule et ajoute une tranche au profil.
inline void WidgetProfil::push(State& state)
    {mProfil.push(state);}
// Accesseurs.
inline void WidgetProfil::setScroll(double scroll)
    {mScroll = scroll;}
inline void WidgetProfil::setLifespan(Time lifespan)
    {mLifespan = lifespan;}
inline void WidgetProfil::setFinished()
    {mFinish = true;}

#endif // WIDGETPROFIL_HPP
