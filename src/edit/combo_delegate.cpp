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

#include "combo_delegate.hpp"

#include <QComboBox>

// Constructeur.
ComboDelegate::ComboDelegate(const QStringList& names) :
    mNames(names)
{
}

// Création d'un QComboBox.
QWidget* ComboDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&/* option*/, const QModelIndex&/* index*/) const
{
    QComboBox* editor = new QComboBox(parent);
    editor->addItems(mNames);
    return editor;
}

// Modification des données.
void ComboDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    // Récupération de la valeur dans le modèle MVC.
    int value = index.model()->data(index, Qt::UserRole).toInt();

    // Application au QComboBox.
    QComboBox* comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentIndex(value);
}

void ComboDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    // Récupération de la valeur dans le QComboBox.
    QComboBox* comboBox = static_cast<QComboBox*>(editor);
    int value = comboBox->currentIndex();

    // Application au modèle MVC.
    model->setData(index, value, Qt::UserRole);
    model->setData(index, mNames[value], Qt::DisplayRole);
}

// Changement de géométrie.
void ComboDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex&/* index*/) const
{
    // Transmission des paramètres.
    editor->setGeometry(option.rect);
}
