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

#ifndef SPINBOX_DELEGATE_HPP
#define SPINBOX_DELEGATE_HPP

#include <QItemDelegate>

// Delegate associé à un QSpinBox pour éditer des nombres entiers.
class SpinBoxDelegate : public QItemDelegate
{
public:
    // Constructeurs.
    SpinBoxDelegate(unsigned int max = 100);

    // Choix de la valeur maximale.
    inline void setMax(unsigned int max);

    // Création d'un QSpinBox.
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    // Modification des données.
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

    // Changement de géométrie.
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
    unsigned int mMax;
};

// Choix de la valeur maximale.
inline void SpinBoxDelegate::setMax(unsigned int max)
    {mMax = max;}

#endif // SPINBOX_DELEGATE_HPP
