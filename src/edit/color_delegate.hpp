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

#ifndef COLOR_DELEGATE_HPP
#define COLOR_DELEGATE_HPP

#include <QtGui>

// Widget permetant d'éditer une couleur.
class ColorEditor : public QWidget
{
public:
    // Constructeur.
    ColorEditor(QWidget* parent = 0);

    // Accesseurs.
    inline void setValue(QColor value);
    inline QColor value() const;

    // Conversion couleur - string.
    static QColor strToColor(QString color);
    static QString strFromColor(QColor color);

private:
    // Pour afficher la couleur en hexadécimal.
    QLineEdit* mEdit;
    // Valide le texte entré dans le QLineEdit.
    QRegExpValidator* mValidator;
};

// Accesseurs.
inline void ColorEditor::setValue(QColor value)
    {mEdit->setText(ColorEditor::strFromColor(value));}
inline QColor ColorEditor::value() const
    {return ColorEditor::strToColor(mEdit->text());}


// Delegate associé au ColorEditor.
class ColorDelegate : public QItemDelegate
{
public:
    // Constructeur.
    ColorDelegate();

    // Création d'un ColorEditor.
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    // Modification des données.
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

    // Changement de géométrie.
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // COLOR_DELEGATE_HPP
