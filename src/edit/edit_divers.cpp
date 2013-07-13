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

#include "edit_divers.hpp"

#include "coord.tpl"

// Constructeur.
EditDivers::EditDivers() :
    mLayout(new QFormLayout),
    mXGrav(new QLineEdit),
    mYGrav(new QLineEdit)
{
    // Validation pour nombre réels.
    mXGrav->setValidator(new QDoubleValidator(mXGrav));
    mYGrav->setValidator(new QDoubleValidator(mYGrav));

    // Remplissage du layout.
    mLayout->addRow(QString::fromUtf8("gravité x"), mXGrav);
    mLayout->addRow(QString::fromUtf8("gravité y"), mYGrav);

    this->setLayout(mLayout);
}


// Accesseurs.
void EditDivers::setGravity(const Coord<double>& gravity)
{
    mXGrav->setText(QString::number(gravity.x));
    mYGrav->setText(QString::number(gravity.y));
}

Coord<double> EditDivers::gravity() const
{
    return Coord<double>(mXGrav->text().toDouble(), mYGrav->text().toDouble());
}
