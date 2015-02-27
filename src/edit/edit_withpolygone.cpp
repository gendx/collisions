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

#include "edit_withpolygone.hpp"

// Constructeur.
EditWithPolygone::EditWithPolygone(EditPolygone* editPolygone) :
    mLayout(new QGridLayout(this)),
    mEditPolygone(editPolygone),
    mButtonAdd(new QPushButton("add")),
    mButtonRemove(new QPushButton("remove")),
    mModel(new QStandardItemModel)
{
    mLayout->setMargin(0);
    mLayout->addWidget(mButtonAdd, 1, 0);
    mLayout->addWidget(mButtonRemove, 1, 1);

    QObject::connect(mButtonAdd, SIGNAL(clicked()), this, SLOT(add()));
    QObject::connect(mButtonRemove, SIGNAL(clicked()), this, SLOT(remove()));
}


// Connexion des signaux et slots.
void EditWithPolygone::showEvent(QShowEvent*/* event*/)
{
    QObject::connect(mEditPolygone, SIGNAL(modified(Polygone, QSet<uint>)), this, SLOT(modified(Polygone)));
}

// Déconnexion des signaux et slots.
void EditWithPolygone::hideEvent(QHideEvent*/* event*/)
{
    QObject::disconnect(mEditPolygone, SIGNAL(modified(Polygone, QSet<uint>)), this, SLOT(modified(Polygone)));
}


// Liste des polygones à dessiner.
QList<Polygone> EditWithPolygone::drawList() const
{
    QList<Polygone> liste = mPolygones;
    if (mIndex != -1 && mIndex < mPolygones.size())
        liste.removeAt(mIndex);
    return liste;
}

// Définit le polygone en cours pour le EditPolygone associé.
void EditWithPolygone::setPolygone()
{
    if (mIndex != -1 && mIndex < mPolygones.size())
        mEditPolygone->setPolygone(mPolygones[mIndex]);
    else
        mEditPolygone->setPolygone();
    emit polygones(this->drawList());
}

// Modifie le polygone en cours.
void EditWithPolygone::modified(Polygone polygone)
{
    if (mIndex != -1 && mIndex < mPolygones.size())
        mPolygones[mIndex] = polygone;
}

// Ajoute un polygone.
void EditWithPolygone::addPolygone()
{
    mIndex = mPolygones.size();
    mPolygones.append(Polygone());
}

// Supprime le polygone en cours.
void EditWithPolygone::removePolygone()
{
    mPolygones.removeAt(mIndex);
    mModel->removeRow(mIndex);
    if (mIndex == mPolygones.size())
        --mIndex;
}
