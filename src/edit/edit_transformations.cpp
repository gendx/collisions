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

#include "edit_transformations.hpp"

#include <QHeaderView>

// Constructeur.
EditTransformations::EditTransformations() :
    mLayout(new QGridLayout(this)),
    mAddReaction(new QPushButton("ajouter")),
    mRemoveReaction(new QPushButton("supprimer")),
    mModelReactions(new QStandardItemModel),
    mReactionsView(new QTableView),
    mAddMutation(new QPushButton("ajouter")),
    mRemoveMutation(new QPushButton("supprimer")),
    mModelMutations(new QStandardItemModel),
    mMutationsView(new QTableView),
    mReactionsList(QStringList() << "rencontre" << QString::fromUtf8("probabilité") << QString::fromUtf8("seuil d'énergie")),
    mMutationsList(QStringList() << QString::fromUtf8("durée") << QString::fromUtf8("probabilité")),
    mSpinboxDelegate(new SpinBoxDelegate),
    mDoubleDelegate(new DoubleDelegate),
    mComboReactions(new ComboDelegate(mReactionsList)),
    mComboMutations(new ComboDelegate(mMutationsList))
{
    // Création de l'interface graphique.
    mLayout->setMargin(0);
    mLayout->addWidget(mReactionsView, 0, 0, 1, 2);
    mLayout->addWidget(mAddReaction, 1, 0);
    mLayout->addWidget(mRemoveReaction, 1, 1);
    mLayout->addWidget(mMutationsView, 2, 0, 1, 2);
    mLayout->addWidget(mAddMutation, 3, 0);
    mLayout->addWidget(mRemoveMutation, 3, 1);

    // Création du modèle MVC (réactions).
    mModelReactions->setColumnCount(6);
    mModelReactions->setHorizontalHeaderLabels(QStringList() << QString::fromUtf8("critère") << "seuil" << "pop src 1" << "pop src 2" << "pop dst 1" << "pop dst 2");

    // Propriétés de la table (réactions).
    mReactionsView->setModel(mModelReactions);
    mReactionsView->setItemDelegateForColumn(0, mComboReactions);
    mReactionsView->setItemDelegateForColumn(1, mDoubleDelegate);
    mReactionsView->setItemDelegateForColumn(2, mSpinboxDelegate);
    mReactionsView->setItemDelegateForColumn(3, mSpinboxDelegate);
    mReactionsView->setItemDelegateForColumn(4, mSpinboxDelegate);
    mReactionsView->setItemDelegateForColumn(5, mSpinboxDelegate);
    mReactionsView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    mReactionsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mReactionsView->setSelectionMode(QAbstractItemView::SingleSelection);

    // Création du modèle MVC (mutations).
    mModelMutations->setColumnCount(4);
    mModelMutations->setHorizontalHeaderLabels(QStringList() << QString::fromUtf8("critère") << "seuil" << "pop. source" << "pop. destination");

    // Propriétés de la table (mutations).
    mMutationsView->setModel(mModelMutations);
    mMutationsView->setItemDelegateForColumn(0, mComboMutations);
    mMutationsView->setItemDelegateForColumn(1, mDoubleDelegate);
    mMutationsView->setItemDelegateForColumn(2, mSpinboxDelegate);
    mMutationsView->setItemDelegateForColumn(3, mSpinboxDelegate);
    mMutationsView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    mMutationsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mMutationsView->setSelectionMode(QAbstractItemView::SingleSelection);


    // Connexion des signaux et slots.
    QObject::connect(mAddReaction, SIGNAL(clicked()), this, SLOT(addReaction()));
    QObject::connect(mRemoveReaction, SIGNAL(clicked()), this, SLOT(removeReaction()));
    QObject::connect(mAddMutation, SIGNAL(clicked()), this, SLOT(addMutation()));
    QObject::connect(mRemoveMutation, SIGNAL(clicked()), this, SLOT(removeMutation()));

    this->connectSelectionReactions(true);
    this->connectSelectionMutations(true);
    mIndexReactions = -1;
    mIndexMutations = -1;
}


// Accesseurs.
void EditTransformations::setReactions(QList<ConfigReaction> reactions)
{
    // Supprime les réactions.
    mModelReactions->removeRows(0, mModelReactions->rowCount());
    mIndexReactions = -1;

    // Ajoute les réactions.
    for (int i = 0 ; i < reactions.size() ; ++i)
    {
        ++mIndexReactions;
        mModelReactions->appendRow(QList<QStandardItem*>() << new QStandardItem(mReactionsList[reactions[i].type()]) << new QStandardItem(QString::number(reactions[i].seuil())) << new QStandardItem(QString::number(reactions[i].pop1())) << new QStandardItem(QString::number(reactions[i].pop2())) << new QStandardItem(QString::number(reactions[i]._pop1())) << new QStandardItem(QString::number(reactions[i]._pop2())));
        mModelReactions->setData(mModelReactions->index(mIndexReactions, 0, QModelIndex()), reactions[i].type(), Qt::UserRole);
    }

    mReactionsView->selectRow(mIndexReactions);
}

void EditTransformations::setMutations(QList<ConfigMutation> mutations)
{
    // Supprime les mutations.
    mModelMutations->removeRows(0, mModelMutations->rowCount());
    mIndexMutations = -1;

    // Ajoute les mutations.
    for (int i = 0 ; i < mutations.size() ; ++i)
    {
        ++mIndexMutations;
        mModelMutations->appendRow(QList<QStandardItem*>() << new QStandardItem(mMutationsList[mutations[i].type()]) << new QStandardItem(QString::number(mutations[i].tau())) << new QStandardItem(QString::number(mutations[i].pop1())) << new QStandardItem(QString::number(mutations[i].pop2())));
        mModelMutations->setData(mModelMutations->index(mIndexMutations, 0, QModelIndex()), mutations[i].type(), Qt::UserRole);
    }

    mMutationsView->selectRow(mIndexMutations);
}

QList<ConfigReaction> EditTransformations::configReactions() const
{
    QList<ConfigReaction> result;

    // Récupération des données dans le modèle MVC.
    for (int i = 0 ; i < mModelReactions->rowCount() ; ++i)
    {
        unsigned int critere = mModelReactions->data(mModelReactions->index(i, 0, QModelIndex()), Qt::UserRole).toUInt();
        double seuil = mModelReactions->data(mModelReactions->index(i, 1, QModelIndex()), Qt::EditRole).toDouble();
        unsigned int pop1 = mModelReactions->data(mModelReactions->index(i, 2, QModelIndex()), Qt::EditRole).toUInt();
        unsigned int pop2 = mModelReactions->data(mModelReactions->index(i, 3, QModelIndex()), Qt::EditRole).toUInt();
        unsigned int _pop1 = mModelReactions->data(mModelReactions->index(i, 4, QModelIndex()), Qt::EditRole).toUInt();
        unsigned int _pop2 = mModelReactions->data(mModelReactions->index(i, 5, QModelIndex()), Qt::EditRole).toUInt();

        ConfigReaction::Type type = ConfigReaction::none;
        if (critere == ConfigReaction::energy)
            type = ConfigReaction::energy;
        else if (critere == ConfigReaction::proba)
            type = ConfigReaction::proba;

        result.append(ConfigReaction(pop1, pop2, type, seuil, _pop1, _pop2));
    }

    return result;
}

QList<ConfigMutation> EditTransformations::configMutations() const
{
    QList<ConfigMutation> result;

    // Récupération des données dans le modèle MVC.
    for (int i = 0 ; i < mModelMutations->rowCount() ; ++i)
    {
        unsigned int critere = mModelMutations->data(mModelMutations->index(i, 0, QModelIndex()), Qt::UserRole).toUInt();
        double tau = mModelMutations->data(mModelMutations->index(i, 1, QModelIndex()), Qt::EditRole).toDouble();
        unsigned int pop1 = mModelMutations->data(mModelMutations->index(i, 2, QModelIndex()), Qt::EditRole).toUInt();
        unsigned int pop2 = mModelMutations->data(mModelMutations->index(i, 3, QModelIndex()), Qt::EditRole).toUInt();

        ConfigMutation::Type type = ConfigMutation::time;
        if (critere == ConfigMutation::proba)
            type = ConfigMutation::proba;

        result.append(ConfigMutation(pop1, pop2, type, tau));
    }

    return result;
}


// Met à jour l'index sélectionné.
void EditTransformations::selectReactions()
{
    QModelIndexList selection = mReactionsView->selectionModel()->selectedIndexes();
    if (selection.isEmpty())
        mIndexReactions = -1;
    else
        mIndexReactions = selection[0].row();
}

// Met à jour l'index sélectionné.
void EditTransformations::selectMutations()
{
    QModelIndexList selection = mMutationsView->selectionModel()->selectedIndexes();
    if (selection.isEmpty())
        mIndexMutations = -1;
    else
        mIndexMutations = selection[0].row();
}


// Active la synchronisation avec la sélection.
void EditTransformations::connectSelectionReactions(bool connect)
{
    if (connect)
        QObject::connect(mReactionsView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectReactions()));
    else
        QObject::disconnect(mReactionsView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectReactions()));
}

void EditTransformations::connectSelectionMutations(bool connect)
{
    if (connect)
        QObject::connect(mMutationsView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectMutations()));
    else
        QObject::disconnect(mMutationsView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectMutations()));
}


// Ajoute une réaction.
void EditTransformations::addReaction()
{
    this->connectSelectionReactions(false);

    mIndexReactions = mModelReactions->rowCount();

    mModelReactions->appendRow(QList<QStandardItem*>() << new QStandardItem(mReactionsList[ConfigReaction::none]) << new QStandardItem(QString::number(0)) << new QStandardItem(QString::number(0)) << new QStandardItem(QString::number(0)) << new QStandardItem(QString::number(0)) << new QStandardItem(QString::number(0)));
    mModelReactions->setData(mModelReactions->index(mIndexReactions, 0, QModelIndex()), ConfigReaction::none, Qt::UserRole);

    mReactionsView->selectRow(mIndexReactions);

    this->connectSelectionReactions(true);
}

// Supprime la réaction sélectionnée.
void EditTransformations::removeReaction()
{
    if (mIndexReactions == -1)
        return;

    this->connectSelectionReactions(false);

    mModelReactions->removeRow(mIndexReactions);
    if (mIndexReactions == mModelReactions->rowCount())
        --mIndexReactions;

    mReactionsView->selectionModel()->select(mModelReactions->index(mIndexReactions, 0, QModelIndex()), QItemSelectionModel::SelectCurrent);

    this->connectSelectionReactions(true);
}

// Ajoute une mutation.
void EditTransformations::addMutation()
{
    this->connectSelectionMutations(false);

    mIndexMutations = mModelMutations->rowCount();

    mModelMutations->appendRow(QList<QStandardItem*>() << new QStandardItem(mMutationsList[ConfigMutation::time]) << new QStandardItem(QString::number(0)) << new QStandardItem(QString::number(0)) << new QStandardItem(QString::number(0)));
    mModelMutations->setData(mModelMutations->index(mIndexMutations, 0, QModelIndex()), ConfigMutation::time, Qt::UserRole);

    mMutationsView->selectRow(mIndexMutations);

    this->connectSelectionMutations(true);
}

// Supprime la mutation sélectionnée.
void EditTransformations::removeMutation()
{
    if (mIndexMutations == -1)
        return;

    this->connectSelectionMutations(false);

    mModelMutations->removeRow(mIndexMutations);
    if (mIndexMutations == mModelMutations->rowCount())
        --mIndexMutations;

    mMutationsView->selectionModel()->select(mModelMutations->index(mIndexMutations, 0, QModelIndex()), QItemSelectionModel::SelectCurrent);

    this->connectSelectionMutations(true);
}
