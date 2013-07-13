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

#ifndef COORD_DELEGATE_HPP
#define COORD_DELEGATE_HPP

#include <QtGui>

class CoordEditor : public QWidget
{
    Q_OBJECT

public:
    CoordEditor(QWidget* parent = 0);

    void setValue(QPointF value);
    QPointF value() const;

private:
    QHBoxLayout* mLayout;
    QLineEdit* mEditX;
    QLineEdit* mEditY;
};

class CoordDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    CoordDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // COORD_DELEGATE_HPP
