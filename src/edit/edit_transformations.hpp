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

#ifndef EDIT_TRANSFORMATIONS_HPP
#define EDIT_TRANSFORMATIONS_HPP

#include "edit_withpolygone.hpp"
#include "spinbox_delegate.hpp"
#include "combo_delegate.hpp"

// Widget pour éditer les transformations de particules.
class EditTransformations : public QWidget
{
    Q_OBJECT

public:
    // Constructeur.
    EditTransformations();

    // Accesseurs.
    void setReactions(QList<ConfigReaction> reactions);
    void setMutations(QList<ConfigMutation> mutations);
    QList<ConfigReaction> configReactions() const;
    QList<ConfigMutation> configMutations() const;

private slots:
    // Met à jour l'index sélectionné.
    void selectMutations();
    void selectReactions();
    // Ajout/suppression.
    void addReaction();
    void removeReaction();
    void addMutation();
    void removeMutation();

private:
    // Active la synchronisation avec la sélection.
    void connectSelectionMutations(bool connect);
    void connectSelectionReactions(bool connect);

    QGridLayout* mLayout;

    // Réaction entre deux particules.
    QPushButton* mAddReaction;
    QPushButton* mRemoveReaction;
    QStandardItemModel* mModelReactions;
    QTableView* mReactionsView;

    // Mutation d'une particule.
    QPushButton* mAddMutation;
    QPushButton* mRemoveMutation;
    QStandardItemModel* mModelMutations;
    QTableView* mMutationsView;

    int mIndexReactions;
    QStringList mReactionsList;
    int mIndexMutations;
    QStringList mMutationsList;

    // Delegates.
    SpinBoxDelegate* mSpinboxDelegate;
    DoubleDelegate* mDoubleDelegate;
    ComboDelegate* mComboReactions;
    ComboDelegate* mComboMutations;
};

#endif // EDIT_TRANSFORMATIONS_HPP
