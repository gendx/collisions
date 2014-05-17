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

#include "edit_populations.hpp"

#include <QHeaderView>

// Constructeur.
EditPopulations::EditPopulations(EditPolygone* editPolygone) :
    EditWithPolygone(editPolygone),
    mTableView(new QTableView),
    mSpinboxDelegate(new SpinBoxDelegate(5000)),
    mNotnullDelegate(new DoubleDelegate(1e-10)),
    mPositiveDelegate(new DoubleDelegate(0)),
    mColorDelegate(new ColorDelegate)
{
    // Création du modèle MVC.
    mModel->setColumnCount(5);
    mModel->setHorizontalHeaderLabels(QStringList() << "nombre" << "rayon" << "masse" << "vitesse" << "couleur");

    // Propriétés de la table.
    mTableView->setModel(mModel);
    // Delegates.
    mTableView->setItemDelegateForColumn(0, mSpinboxDelegate);
    mTableView->setItemDelegateForColumn(1, mNotnullDelegate);
    mTableView->setItemDelegateForColumn(2, mNotnullDelegate);
    mTableView->setItemDelegateForColumn(3, mPositiveDelegate);
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
void EditPopulations::setPopulations(QList<ConfigPopulation> populations)
{
    this->connectSelection(false);

    // Supprime les populations.
    mModel->removeRows(0, mModel->rowCount());
    mPolygones.clear();
    mIndex = 0;

    // Ajoute les populations.
    for (auto& population : populations)
    {
        this->addPolygone();
        mPolygones.back() = population.polygone();

        QString name = ColorEditor::strFromColor(population.color());
        mModel->appendRow(QList<QStandardItem*>() << new QStandardItem(QString::number(population.taille())) << new QStandardItem(QString::number(population.rayon())) << new QStandardItem(QString::number(population.masse())) << new QStandardItem(QString::number(population.vitesse())) << new QStandardItem(name));
        mModel->setData(mModel->index(mIndex, 4, QModelIndex()), population.color(), Qt::DecorationRole);
        mModel->setData(mModel->index(mIndex, 4, QModelIndex()), population.color(), Qt::UserRole);
    }

    mTableView->selectRow(mIndex);
    this->connectSelection(true);
}

QList<ConfigPopulation> EditPopulations::config() const
{
    QList<ConfigPopulation> result;

    // Récupération des données dans le modèle MVC.
    for (int i = 0 ; i < mPolygones.size() ; ++i)
    {
        unsigned int nombre = mModel->data(mModel->index(i, 0, QModelIndex()), Qt::EditRole).toUInt();
        double rayon = mModel->data(mModel->index(i, 1, QModelIndex()), Qt::EditRole).toDouble();
        double masse = mModel->data(mModel->index(i, 2, QModelIndex()), Qt::EditRole).toDouble();
        double vitesse = mModel->data(mModel->index(i, 3, QModelIndex()), Qt::EditRole).toDouble();
        QColor couleur = QColor(mModel->data(mModel->index(i, 4, QModelIndex()), Qt::DecorationRole).toString());
        result.append(ConfigPopulation(nombre, rayon, masse, couleur, mPolygones[i], vitesse));
    }

    return result;
}


// Met à jour le polygone selon la sélection.
void EditPopulations::selectPolygone()
{
    QModelIndexList selection = mTableView->selectionModel()->selectedIndexes();
    if (selection.isEmpty())
        mIndex = -1;
    else
        mIndex = selection[0].row();
    this->setPolygone();
}


// Active la synchronisation avec la sélection.
void EditPopulations::connectSelection(bool connect)
{
    if (connect)
        QObject::connect(mTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectPolygone()));
    else
        QObject::disconnect(mTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectPolygone()));
}

// Ajoute une population.
void EditPopulations::add()
{
    this->addPolygone();
    this->connectSelection(false);

    mModel->appendRow(QList<QStandardItem*>() << new QStandardItem(QString::number(10)) << new QStandardItem(QString::number(1)) << new QStandardItem(QString::number(1)) << new QStandardItem(QString::number(1)) << new QStandardItem("black"));
    mModel->setData(mModel->index(mIndex, 4, QModelIndex()), Qt::black, Qt::DecorationRole);
    mModel->setData(mModel->index(mIndex, 4, QModelIndex()), QColor(Qt::black).name(), Qt::UserRole);

    // Sélectionne la nouvelle population.
    mTableView->selectRow(mIndex);
    this->setPolygone();

    this->connectSelection(true);
}

// Supprime la population sélectionnée.
void EditPopulations::remove()
{
    if (mIndex == -1)
        return;

    this->connectSelection(false);

    // Suppression.
    this->removePolygone();
    mTableView->selectionModel()->select(mModel->index(mIndex, 0, QModelIndex()), QItemSelectionModel::SelectCurrent);
    this->setPolygone();

    this->connectSelection(true);
}
