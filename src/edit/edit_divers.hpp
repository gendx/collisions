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

#ifndef EDIT_DIVERS_HPP
#define EDIT_DIVERS_HPP

#include <QFormLayout>
#include "double_delegate.hpp"
#include "coord.hpp"

// Widget pour éditer les paramètres divers (gravité).
class EditDivers : public QWidget
{
public:
    // Constructeur.
    EditDivers();

    // Accesseurs.
    void setGravity(const Coord<double>& gravity);
    Coord<double> gravity() const;

private:
    QFormLayout* mLayout;
    // Gravité.
    QLineEdit* mXGrav;
    QLineEdit* mYGrav;
};

#endif // EDIT_DIVERS_HPP
