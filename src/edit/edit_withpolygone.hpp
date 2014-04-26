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

#ifndef EDIT_WITHPOLYGONE_HPP
#define EDIT_WITHPOLYGONE_HPP

#include <QPushButton>
#include "edit_polygone.hpp"

// Classe abstraite pour les widgets d'édition liés à des polygones.
class EditWithPolygone : public QWidget
{
    Q_OBJECT

public:
    // Constructeur.
    EditWithPolygone(EditPolygone* editPolygone);

    // Liste des polygones à dessiner.
    QList<Polygone> drawList() const;
    // Définit le polygone en cours pour le EditPolygone associé.
    void setPolygone();

signals:
    // Liste des polygones modifiée.
    void polygones(QList<Polygone>);

private slots:
    // Modifie le polygone en cours.
    void modified(Polygone polygone);
    // Ajoute/supprime un élément associé à un polygone.
    virtual void add() = 0;
    virtual void remove() = 0;

private:
    // (Dé)connexion des signaux et slots.
    void showEvent(QShowEvent*/* event*/);
    void hideEvent(QHideEvent*/* event*/);

protected:
    // Ajoute un widget de vue au layout.
    inline void addViewWidget(QWidget* widget);

    // Ajoute un polygone.
    void addPolygone();
    // Supprime le polygone en cours.
    void removePolygone();

    QList<Polygone> mPolygones;
    int mIndex;

    QGridLayout* mLayout;
    EditPolygone* mEditPolygone;
    QPushButton* mButtonAdd;
    QPushButton* mButtonRemove;
    QStandardItemModel* mModel;
};

// Ajoute un widget de vue au layout.
inline void EditWithPolygone::addViewWidget(QWidget* widget)
    {mLayout->addWidget(widget, 0, 0, 1, 2);}

#endif // EDIT_WITHPOLYGONE_HPP
