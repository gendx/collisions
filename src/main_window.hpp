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

#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QtGui>
#include "document.hpp"

// Fenêtre principale du programme.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructeur.
    MainWindow();

private slots:
    // Slots associés aux actions des menus.
    void newFile();
    void open();
    void save();
    void saveAs();
    void saveAll();
    void closeAll();

    void setEditMode();
    void exportConfig();

    void setSimulMode();
    void play();
    void restart();

    void tileSubwin();
    void cascadeSubwin();
    void tabSubwin();

    // Mise à jour des actions disponibles.
    void updateActions();
    // Mise à jour de la liste des fenêtres.
    void updateWindowMenu();
    // Crée un document vide.
    Document* createDocument();
    // Active un document.
    void setActiveDocument(QWidget* window);

private slots:
    // Mise à jour de la barre de statut.
    void statusText(QString text);

private:
    // Evénements.
    void closeEvent(QCloseEvent* event);

    // Crée les actions pour les menus.
    void createActions();

    // Recherche de documents.
    Document* activeDocument();
    QMdiSubWindow* findDocument(const QString& path);


    // Interface multi-documents.
    QMdiArea* mMdi;
    QSignalMapper* mSignalMapper;

    // Menus.
    QMenu* mFileMenu;
    QAction* mNewAction;
    QAction* mOpenAction;
    QAction* mSaveAction;
    QAction* mSaveasAction;
    QAction* mSaveallAction;
    QAction* mCloseAction;
    QAction* mCloseallAction;

    QMenu* mEditMenu;
    QAction* mEditModeAction;
    QAction* mExportConfigAction;

    QMenu* mSimulMenu;
    QAction* mSimulModeAction;
    QAction* mPlayAction;
    QAction* mRestartAction;

    QMenu* mWindowMenu;
    QAction* mTileAction;
    QAction* mCascadeAction;
    QAction* mTabAction;
    QAction* mPreviousAction;
    QAction* mNextAction;
    QAction* mSeparatorAction;

    QToolBar* mFileToolbar;
    QToolBar* mEditToolbar;
    QToolBar* mSimulToolbar;
};

#endif // MAIN_WINDOW_HPP
