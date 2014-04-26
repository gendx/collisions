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

#include "double_delegate.hpp"

#include <QLineEdit>

// Constructeurs.
DoubleDelegate::DoubleDelegate() :
    mHasMin(false)
{
}

DoubleDelegate::DoubleDelegate(double min) :
    mHasMin(true),
    mMin(min)
{
}

// Création d'un QLineEdit.
QWidget* DoubleDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&/* option*/, const QModelIndex&/* index*/) const
{
    QLineEdit* editor = new QLineEdit(parent);
    QDoubleValidator* validator = new QDoubleValidator(editor);

    if (mHasMin)
        validator->setBottom(mMin);
    editor->setValidator(validator);

    return editor;
}

// Modification des données.
void DoubleDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    // Récupération de la valeur dans le modèle MVC.
    double value = index.model()->data(index, Qt::EditRole).toDouble();

    // Application au QLineEdit.
    QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(QString::number(value));
}

void DoubleDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    // Récupération de la valeur dans le QLineEdit.
    QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
    double value = lineEdit->text().toDouble();

    if (mHasMin && value < mMin)
        return;

    // Application au modèle MVC.
    model->setData(index, value, Qt::EditRole);
}

// Changement de géométrie.
void DoubleDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex&/* index*/) const
{
    // Transmission des paramètres.
    editor->setGeometry(option.rect);
}
