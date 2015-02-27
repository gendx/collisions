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

#include "edit_obstacles.hpp"

#include <QMessageBox>

// Constructeur.
EditObstacles::EditObstacles(EditPolygone* editPolygone) :
    EditWithPolygone(editPolygone),
    mListView(new QListView)
{
    // Liste des obstacles.
    mListView->setModel(mModel);
    this->addViewWidget(mListView);

    this->connectSelection(true);

    // Ajoute le contour.
    QStandardItem* item = new QStandardItem("contour");
    item->setEditable(false);
    mModel->appendRow(QList<QStandardItem*>() << item);
    mPolygones.append(Polygone());

    mIndex = 0;
    mListView->selectionModel()->select(mModel->index(0, 0, QModelIndex()), QItemSelectionModel::Select);
}


// Accesseurs.
void EditObstacles::setContour(Obstacle contour)
{
    mPolygones[0] = contour.sommets();
}

void EditObstacles::setObstacles(QList<Obstacle> obstacles)
{
    this->connectSelection(false);

    // Vide la liste des obstacles.
    mIndex = mPolygones.size() - 1;
    while (mIndex > 0)
        this->removePolygone();

    // Ajoute les obstacles.
    for (auto& obstacle : obstacles)
    {
        this->addPolygone();
        this->addObstacle();
        mPolygones[mIndex] = obstacle.sommets();
    }

    // Sélectionne l'obstacle en cours.
    mListView->selectionModel()->select(mModel->index(mIndex, 0, QModelIndex()), QItemSelectionModel::SelectCurrent);
    if (mIndex > 0)
        mListView->selectionModel()->select(mModel->index(0, 0, QModelIndex()), QItemSelectionModel::Deselect);

    // Synchronisation du polygone avec l'obstacle.
    this->setPolygone();
    this->connectSelection(true);
}


Obstacle EditObstacles::contour() const
{
    return Obstacle(mPolygones[0], QColor(0xC0, 0xC0, 0xE0));
}

QList<Obstacle> EditObstacles::obstacles() const
{
    QList<Obstacle> result;

    for (int i = 1 ; i < mPolygones.size() ; ++i)
        result.append(Obstacle(mPolygones[i], QColor(0xC0, 0xC0, 0xE0)));

    return result;
}


// Met à jour le polygone selon la sélection.
void EditObstacles::selectPolygone()
{
    QModelIndexList selection = mListView->selectionModel()->selectedIndexes();
    if (!selection.isEmpty())
        mIndex = selection[0].row();
    this->setPolygone();
}


// Active la synchronisation du polygone avec la sélection.
void EditObstacles::connectSelection(bool connect)
{
    if (connect)
        QObject::connect(mListView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectPolygone()));
    else
        QObject::disconnect(mListView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectPolygone()));
}

// Ajoute un obstacle.
void EditObstacles::add()
{
    unsigned int old = mIndex;
    this->addPolygone();

    this->connectSelection(false);

    this->addObstacle();

    // Sélectionne le nouvel obstacle.
    mListView->selectionModel()->select(mModel->index(mIndex, 0, QModelIndex()), QItemSelectionModel::SelectCurrent);
    mListView->selectionModel()->select(mModel->index(old, 0, QModelIndex()), QItemSelectionModel::Deselect);
    this->setPolygone();

    this->connectSelection(true);
}

// Ajoute un obtacle à la liste.
void EditObstacles::addObstacle()
{
    static unsigned int i = 0;

    QStandardItem* item = new QStandardItem(QString("obstacle ") + QString::number(++i));
    item->setEditable(false);
    mModel->appendRow(QList<QStandardItem*>() << item);
}

// Supprime l'obstacle sélectionné.
void EditObstacles::remove()
{
    if (mIndex == -1)
        return;
    if (mIndex == 0)
    {
        QMessageBox::warning(this, "Error", "Cannot remove the contour !");
        return;
    }

    this->connectSelection(false);

    // Suppression.
    this->removePolygone();
    mListView->selectionModel()->select(mModel->index(mIndex, 0, QModelIndex()), QItemSelectionModel::SelectCurrent);
    this->setPolygone();

    this->connectSelection(true);
}
