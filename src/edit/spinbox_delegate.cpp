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

#include "spinbox_delegate.hpp"

// Constructeurs.
SpinBoxDelegate::SpinBoxDelegate(unsigned int max) :
    mMax(max)
{
}

// Création d'un QSpinBox.
QWidget* SpinBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&/* option*/, const QModelIndex&/* index*/) const
{
    QSpinBox* editor = new QSpinBox(parent);
    editor->setMinimum(0);
    editor->setMaximum(mMax);

    return editor;
}

// Modification des données.
void SpinBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    // Récupération de la valeur dans le modèle MVC.
    int value = index.model()->data(index, Qt::EditRole).toInt();

    // Application au QSpinBox.
    QSpinBox* spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    // Récupération de la valeur dans le QSpinBox.
    QSpinBox* spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();

    // Application au modèle MVC.
    model->setData(index, value, Qt::EditRole);
}

// Changement de géométrie.
void SpinBoxDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex&/* index*/) const
{
    // Transmission des paramètres.
    editor->setGeometry(option.rect);
}
