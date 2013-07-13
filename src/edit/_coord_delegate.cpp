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

#include "coord_delegate.hpp"

CoordEditor::CoordEditor(QWidget* parent) :
    QWidget(parent),
    mLayout(new QHBoxLayout),
    mEditX(new QLineEdit),
    mEditY(new QLineEdit)
{
    QDoubleValidator* validator = new QDoubleValidator(mEditX);
    mEditX->setValidator(validator);
    validator = new QDoubleValidator(mEditY);
    mEditY->setValidator(validator);

    mLayout->addWidget(mEditX);
    mLayout->addWidget(mEditY);
    mLayout->setMargin(0);
    mLayout->setSpacing(2);
    this->setLayout(mLayout);
}

void CoordEditor::setValue(QPointF value)
{
    mEditX->setText(QString::number(value.x()));
    mEditY->setText(QString::number(value.y()));
}

QPointF CoordEditor::value() const
{
    return QPointF(mEditX->text().toDouble(), mEditY->text().toDouble());
}



CoordDelegate::CoordDelegate()
{
}

QWidget* CoordDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&/* option*/, const QModelIndex&/* index*/) const
{
    CoordEditor* editor = new CoordEditor(parent);
    return editor;
}

void CoordDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QPointF value = index.model()->data(index, Qt::UserRole).toPointF();

    CoordEditor* coordEdit = static_cast<CoordEditor*>(editor);
    coordEdit->setValue(value);
}

void CoordDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    CoordEditor* coordEdit = static_cast<CoordEditor*>(editor);
    QPointF value = coordEdit->value();
    QString str = QString::number(value.x()) + " ; " + QString::number(value.y());

    model->setData(index, value, Qt::UserRole);
    model->setData(index, str, Qt::DisplayRole);
}

void CoordDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex&/* index*/) const
{
    editor->setGeometry(option.rect);
}
