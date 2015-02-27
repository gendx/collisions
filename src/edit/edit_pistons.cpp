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

#include "edit_pistons.hpp"

#include <QHeaderView>

// Constructeur.
EditPistons::EditPistons(EditPolygone* editPolygone) :
    EditWithPolygone(editPolygone),
    mTableView(new QTableView),
    mPositiveDelegate(new DoubleDelegate(0)),
    mNotnullDelegate(new DoubleDelegate(1e-10)),
    mDoubleDelegate(new DoubleDelegate),
    mColorDelegate(new ColorDelegate)
{
    // Création du modèle MVC.
    mModel->setColumnCount(5);
    mModel->setHorizontalHeaderLabels(QStringList() << "epaisseur" << "masse" << "position" << "vitesse" << "couleur");

    // Propriétés de la table.
    mTableView->setModel(mModel);
    // Delegates.
    mTableView->setItemDelegateForColumn(0, mPositiveDelegate);
    mTableView->setItemDelegateForColumn(1, mNotnullDelegate);
    mTableView->setItemDelegateForColumn(2, mDoubleDelegate);
    mTableView->setItemDelegateForColumn(3, mDoubleDelegate);
    mTableView->setItemDelegateForColumn(4, mColorDelegate);
    // Divers.
    mTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    mTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    this->addViewWidget(mTableView);

    this->connectSelection(true);
    mIndex = -1;
}


// Accesseurs.
void EditPistons::setPistons(QList<ConfigPiston> pistons)
{
    this->connectSelection(false);

    // Supprime les pistons.
    mModel->removeRows(0, mModel->rowCount());
    mIndex = -1;

    // Ajoute les pistons.
    for (auto& piston : pistons)
    {
        ++mIndex;
        QString name = ColorEditor::strFromColor(piston.mColor);
        mModel->appendRow(QList<QStandardItem*>() << new QStandardItem(QString::number(piston.mEpaisseur)) << new QStandardItem(QString::number(piston.mMasse)) << new QStandardItem(QString::number(piston.mPosition)) << new QStandardItem(QString::number(piston.mVitesse)) << new QStandardItem(name));
        mModel->setData(mModel->index(mIndex, 4, QModelIndex()), piston.mColor, Qt::DecorationRole);
        mModel->setData(mModel->index(mIndex, 4, QModelIndex()), piston.mColor, Qt::UserRole);
    }

    mTableView->selectRow(mIndex);
    this->connectSelection(true);
}

QList<ConfigPiston> EditPistons::config() const
{
    QList<ConfigPiston> result;

    // Récupération des données dans le modèle MVC.
    for (int i = 0 ; i < mModel->rowCount() ; ++i)
    {
        double epaisseur = mModel->data(mModel->index(i, 0, QModelIndex()), Qt::EditRole).toDouble();
        double masse = mModel->data(mModel->index(i, 1, QModelIndex()), Qt::EditRole).toDouble();
        double position = mModel->data(mModel->index(i, 2, QModelIndex()), Qt::EditRole).toDouble();
        double vitesse = mModel->data(mModel->index(i, 3, QModelIndex()), Qt::EditRole).toDouble();
        QColor couleur = ColorEditor::strToColor(mModel->data(mModel->index(i, 4, QModelIndex()), Qt::UserRole).toString());
        result.append(ConfigPiston(position, vitesse, couleur, masse, epaisseur));
    }

    return result;
}


// Met à jour l'index en cours selon la sélection.
void EditPistons::select()
{
    QModelIndexList selection = mTableView->selectionModel()->selectedIndexes();
    if (selection.isEmpty())
        mIndex = -1;
    else
        mIndex = selection[0].row();
}

// Active la synchronisation avec la sélection.
void EditPistons::connectSelection(bool connect)
{
    if (connect)
        QObject::connect(mTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(select()));
    else
        QObject::disconnect(mTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(select()));
}

// Ajoute un piston.
void EditPistons::add()
{
    ++mIndex;
    this->connectSelection(false);

    // Ajout.
    mModel->appendRow(QList<QStandardItem*>() << new QStandardItem(QString::number(10)) << new QStandardItem(QString::number(1)) << new QStandardItem(QString::number(0)) << new QStandardItem(QString::number(0)) << new QStandardItem("black"));
    mModel->setData(mModel->index(mIndex, 4, QModelIndex()), Qt::black, Qt::DecorationRole);
    mModel->setData(mModel->index(mIndex, 4, QModelIndex()), QColor(Qt::black).name(), Qt::UserRole);

    // Sélection.
    mTableView->selectRow(mIndex);

    this->connectSelection(true);
}

// Supprime le piston sélectionné.
void EditPistons::remove()
{
    if (mIndex == -1)
        return;

    this->connectSelection(false);

    // Suppression.
    mModel->removeRow(mIndex);
    if (mIndex == mModel->rowCount())
        --mIndex;

    mTableView->selectionModel()->select(mModel->index(mIndex, 0, QModelIndex()), QItemSelectionModel::SelectCurrent);

    this->connectSelection(true);
}
