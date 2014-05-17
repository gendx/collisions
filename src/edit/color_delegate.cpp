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

#include "color_delegate.hpp"

// Constructeur.
ColorEditor::ColorEditor(QWidget* parent) :
    QWidget(parent),
    mEdit(new QLineEdit(this)),
    // Valide 6 chiffres hexadécimaux.
    mValidator(new QRegExpValidator(QRegExp("^([0-9]|[a-f]|[A-F]){6}$"), this))
{
    mEdit->setValidator(mValidator);
}


// Conversion couleur - string.
QColor ColorEditor::strToColor(QString color)
{
    unsigned int nbr = color.toLong(0, 16);
    return QColor((nbr >> 16) & 0xFF, (nbr >> 8) & 0xFF, nbr & 0xFF);
}

QString ColorEditor::strFromColor(QColor color)
{
    QString result = QString::number((color.red() << 16) + (color.green() << 8) + color.blue(), 16);
    result = result.toUpper();
    while (result.size() < 6)
        result.prepend('0');
    return result;
}



// Constructeur.
ColorDelegate::ColorDelegate()
{
}

// Création d'un ColorEditor.
QWidget* ColorDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&/* option*/, const QModelIndex&/* index*/) const
{
    ColorEditor* editor = new ColorEditor(parent);
    return editor;
}

// Modification des données.
void ColorDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    // Récupération de la valeur dans le modèle MVC.
    QColor value = index.model()->data(index, Qt::UserRole).toString();

    // Application au ColorEditor.
    ColorEditor* colorEdit = static_cast<ColorEditor*>(editor);
    colorEdit->setValue(value);
}

void ColorDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    // Récupération de la valeur dans le ColorEditor.
    ColorEditor* colorEdit = static_cast<ColorEditor*>(editor);
    QColor value = colorEdit->value();
    QString str = ColorEditor::strFromColor(value);

    // Choix d'un nom de couleur user-friendly.
    for (auto& name : QColor::colorNames())
        if (value == QColor(name))
            str = name;

    // Application au modèle MVC.
    model->setData(index, value.name(), Qt::UserRole);
    model->setData(index, str, Qt::DisplayRole);
    model->setData(index, value, Qt::DecorationRole);
}

// Changement de géométrie.
void ColorDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex&/* index*/) const
{
    // Transmission des paramètres.
    editor->setGeometry(option.rect);
}
