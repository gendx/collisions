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

#ifndef EDIT_POPULATIONS_HPP
#define EDIT_POPULATIONS_HPP

#include "edit_withpolygone.hpp"
#include "spinbox_delegate.hpp"
#include "color_delegate.hpp"

// Widget pour éditer les populations de particules.
class EditPopulations : public EditWithPolygone
{
    Q_OBJECT

public:
    // Constructeur.
    EditPopulations(EditPolygone* editPolygone);

    // Accesseurs.
    void setPopulations(QList<ConfigPopulation> populations);
    QList<ConfigPopulation> config() const;

public slots:
    // Met à jour le polygone selon la sélection.
    void selectPolygone();

private slots:
    // Ajoute une population.
    void add();
    // Supprime la population sélectionnée.
    void remove();

private:
    // Active la synchronisation avec la sélection.
    void connectSelection(bool connect);

    QTableView* mTableView;

    SpinBoxDelegate* mSpinboxDelegate;
    DoubleDelegate* mNotnullDelegate;
    DoubleDelegate* mPositiveDelegate;
    ColorDelegate* mColorDelegate;
};

#endif // EDIT_POPULATIONS_HPP
