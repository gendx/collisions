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

#ifndef EDIT_COURBES_HPP
#define EDIT_COURBES_HPP

#include <QWidget>

// Widget pour éditer les courbes à afficher (non opérationnel).
class EditCourbes : public QWidget
{
public:
    // Constructeur.
    explicit EditCourbes(QWidget *parent = 0);

};

#endif // EDIT_COURBES_HPP
