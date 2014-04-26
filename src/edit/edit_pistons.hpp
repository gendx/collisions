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

#ifndef EDIT_PISTONS_HPP
#define EDIT_PISTONS_HPP

#include "edit_withpolygone.hpp"
#include "spinbox_delegate.hpp"
#include "color_delegate.hpp"

// Widget pour éditer les pistons.
class EditPistons : public EditWithPolygone
{
    Q_OBJECT

public:
    // Constructeur.
    EditPistons(EditPolygone* editPolygone);

    // Accesseurs.
    void setPistons(QList<ConfigPiston> pistons);
    QList<ConfigPiston> config() const;

public slots:
    // Met à jour l'index en cours selon la sélection.
    void select();

private slots:
    // Ajout d'un piston.
    void add();
    // Supprime le piston sélectionné.
    void remove();

private:
    // Active la synchronisation avec la sélection.
    void connectSelection(bool connect);

    QTableView* mTableView;

    DoubleDelegate* mPositiveDelegate;
    DoubleDelegate* mNotnullDelegate;
    DoubleDelegate* mDoubleDelegate;
    ColorDelegate* mColorDelegate;
};

#endif // EDIT_PISTONS_HPP
