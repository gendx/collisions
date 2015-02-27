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

#ifndef DOUBLE_DELEGATE_HPP
#define DOUBLE_DELEGATE_HPP

#include <QItemDelegate>

// Delegate associé à un QLineEdit pour éditer des nombres réels.
class DoubleDelegate : public QItemDelegate
{
public:
    // Constructeurs.
    DoubleDelegate();
    DoubleDelegate(double min);

    // Création d'un QLineEdit.
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    // Modification des données.
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

    // Changement de géométrie.
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
    bool mHasMin;
    double mMin;
};

#endif // DOUBLE_DELEGATE_HPP
