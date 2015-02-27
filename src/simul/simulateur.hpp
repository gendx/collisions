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

#ifndef SIMULATEUR_HPP
#define SIMULATEUR_HPP

#include <QLabel>
#include <QSlider>
#include "courbes_group.hpp"
#include "state.hpp"

// Widget pour simuler une configuration.
class Simulateur : public QWidget
{
    Q_OBJECT

public:
    // Constructeur.
    Simulateur(const Configuration& config);

    // Redémarre la simulation.
    void doRestart();
    // Avance jusqu'au prochain événement de dessin.
    void playToNextDraw();
    // Avance jusqu'au prochain événement et effectue tous les événements de cette date.
    bool playNext();

    // Dessine l'état actuel de la simulation.
    void draw(QPainter& painter, double width);

    // Effectue un événement.
    bool performCollision(Collision& collision);
    bool performDrawEvent();
    bool performValueEvent();
    bool performCourbeEvent();
    bool performBouleEvent(Boule* boule);
    // Ajoute un événement.
    void addDrawEvent();
    void addValueEvent();
    void addCourbeEvent();

signals:
    // Requiert de redessiner le Graph.
    void draw();
    void fullDraw();
    // Statut à afficher dans une QStatusBar.
    void statusText(QString);

private slots:
    // Change la fréquence d'affichage.
    void setVitesse(int value);
    //void setArea(int value);
    void setValues(int value);
    void setCourbes(int value);

private:
    // Génère un texte pour la barre de statut (images par seconde, etc).
    void emitStatusText(unsigned int msec, unsigned int frames, unsigned int chocs, unsigned int chocsTotal);

    // Met à jour les collisions en partant des mobiles concernés par la(les) dernière(s) effectuée(s).
    void refreshCollisions();
    // Met à jour les événements de dessin (supprime ceux qui viennent d'être effectués).
    void refreshDrawings();

    // Avance la simulation à un instant donné.
    void avance(const Time& time);

    /*
    // Vérifie que la simulation est cohérente (pas de chevauchements).
    bool check() const;
    //*/

    // Widgets.
    QGridLayout* mLayout;
    CourbesGroup* mGroupCourbes;
    QLabel* mLabelVitesse;
    QSlider* mSliderVitesse;
    QLabel* mLabelValues;
    QSlider* mSliderValues;
    QLabel* mLabelCourbes;
    QSlider* mSliderCourbes;

    State mState;
};

#endif // SIMULATEUR_HPP
