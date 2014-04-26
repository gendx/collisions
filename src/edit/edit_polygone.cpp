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

#include "edit_polygone.hpp"

#include <QHeaderView>
#include <QKeyEvent>

// Constructeur.
EditPolygone::EditPolygone() :
    mHasHover(false),
    mLayout(new QGridLayout(this)),
    mTableView(new QTableView),
    mModel(new QStandardItemModel),
    mCheckGrid(new QCheckBox("Aligner sur la grille")),
    mDelegate(new DoubleDelegate)
{
    // Création du modèle MVC.
    mModel->setRowCount(2);
    mModel->setVerticalHeaderLabels(QStringList() << "x" << "y");

    // Propriétés de la table.
    mTableView->setModel(mModel);
    // Delegate.
    mTableView->setItemDelegate(mDelegate);
    // Divers.
    mTableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    mTableView->setSelectionBehavior(QAbstractItemView::SelectColumns);

    mCheckGrid->setChecked(true);

    mLayout->setMargin(0);
    mLayout->addWidget(mCheckGrid, 0, 0);
    mLayout->addWidget(mTableView, 1, 0);

    // Connexion des signaux et slots.
    QObject::connect(mModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(dataChanged(QStandardItem*)));
    QObject::connect(mTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(sendPolygone()));
}


// Accesseurs.
void EditPolygone::setPolygone(Polygone polygone)
{
    mActive = true;
    this->setEnabled(true);

    mPoints = polygone.sommets();
    mModel->removeColumns(0, mModel->columnCount());

    // Ajout de chaque point au modèle.
    for (int i = 0 ; i < mPoints.size() ; ++i)
        mModel->appendColumn(QList<QStandardItem*>() << new QStandardItem(QString::number(mPoints[i].x)) << new QStandardItem(QString::number(mPoints[i].y)));

    // Envoie le polygone.
    this->sendPolygone();
}

void EditPolygone::setPolygone()
{
    mActive = false;
    this->setEnabled(false);

    // Polygone vide.
    mPoints.clear();
    mModel->removeColumns(0, mModel->columnCount());
    emit modified(Polygone(), QSet<unsigned int>());
}


// Emet le signal "modified" selon les points et la sélection.
void EditPolygone::sendPolygone()
{
    // Liste les points sélectionnés.
    QModelIndexList selected = mTableView->selectionModel()->selectedColumns();
    QSet<unsigned int> selection;
    for (int i = 0 ; i < selected.size() ; ++i)
        selection.insert(selected[i].column());

    // Point survolé.
    QList<Coord<double> > points(mPoints);
    if (mHasHover)
        points.append(mHoverPoint);

    // Emet le signal.
    emit modified(Polygone(points), selection);
}

// Ajoute un point au polygone.
void EditPolygone::point(Coord<double> point, Coord<double> normalized)
{
    if (!mActive)
        return;

    // Point de la grille le plus proche si nécessaire.
    if (mCheckGrid->isChecked())
        point = normalized;

    // Ajoute le point au modèle.
    mModel->appendColumn(QList<QStandardItem*>() << new QStandardItem(QString::number(point.x)) << new QStandardItem(QString::number(point.y)));
    mHasHover = false;
    mPoints.append(point);

    // Envoie le polygone.
    this->sendPolygone();
}

// Point survolé par la souris.
void EditPolygone::hover(Coord<double> point, Coord<double> normalized)
{
    if (!mActive)
        return;

    // Point de la grille le plus proche si nécessaire.
    if (mCheckGrid->isChecked())
        point = normalized;

    // Modifie le point en cours de survol.
    mHasHover = true;
    mHoverPoint = point;
    emit statusText(QString("point (") + QString::number(point.x) + " , " + QString::number(point.y) + ")");

    // Envoie le polygone.
    this->sendPolygone();
}

// Efface le point survolé.
void EditPolygone::noHover()
{
    mHasHover = false;
    emit statusText("");
    this->sendPolygone();
}

// En cas de modification du modèle.
void EditPolygone::dataChanged(QStandardItem* item)
{
    // Récupère les valeurs dans le modèle.
    double value = item->text().toDouble();
    QModelIndex index = item->index();

    if (index.row() == 0)
        mPoints[index.column()].x = value;
    else if (index.row() == 1)
        mPoints[index.column()].y = value;

    // Envoie le polygone.
    this->sendPolygone();
}

// Gestion de la touche suppression.
void EditPolygone::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete)
    {
        // Liste des colonnes sélectionnées.
        QModelIndexList indexes = mTableView->selectionModel()->selectedColumns();
        qSort(indexes.begin(), indexes.end());

        // Supprime tous les points concernés.
        for (int i = indexes.size() - 1 ; i >= 0 ; --i)
        {
            unsigned int index = indexes[i].column();
            mPoints.removeAt(index);
            mModel->removeColumn(index);
        }

        event->accept();

        // Envoie le polygone.
        this->sendPolygone();
    }
}
