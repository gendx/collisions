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

#include "main_window.hpp"

#include <QAction>
#include <QMdiArea>
#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>

// Constructeur.
MainWindow::MainWindow() :
    mMdi(new QMdiArea),
    mFileToolbar(this->addToolBar("&Fichier")),
    mEditToolbar(this->addToolBar("&Edition")),
    mSimulToolbar(this->addToolBar("&Simulation"))
{
    // Création de l'interface graphique.
    this->createActions();

    this->setWindowTitle("Collisions");

    this->setCentralWidget(mMdi);
    mMdi->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mMdi->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->tabSubwin();

    // Création des actions
    this->updateActions();
    this->updateWindowMenu();

    mSeparatorAction->setSeparator(true);

    mFileToolbar->setIconSize(QSize(16, 16));
    mEditToolbar->setIconSize(QSize(16, 16));
    mSimulToolbar->setIconSize(QSize(16, 16));

    mFileToolbar->addAction(mNewAction);
    mFileToolbar->addAction(mOpenAction);
    mFileToolbar->addAction(mSaveAction);
    mFileToolbar->addAction(mSaveasAction);
    mFileToolbar->addAction(mSaveallAction);
    mFileToolbar->addAction(mCloseAction);
    mFileToolbar->addAction(mCloseallAction);

    mEditToolbar->addAction(mEditModeAction);
    mEditToolbar->addAction(mExportConfigAction);

    mSimulToolbar->addAction(mSimulModeAction);
    mSimulToolbar->addAction(mPlayAction);
    mSimulToolbar->addAction(mRestartAction);

    // Connexion des signaux et slots.
    QObject::connect(mMdi, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateActions()));
    mSignalMapper = new QSignalMapper(this);
    QObject::connect(mSignalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveDocument(QWidget*)));

    QObject::connect(mWindowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    QObject::connect(mNewAction, SIGNAL(triggered()), this, SLOT(newFile()));
    QObject::connect(mOpenAction, SIGNAL(triggered()), this, SLOT(open()));
    QObject::connect(mSaveAction, SIGNAL(triggered()), this, SLOT(save()));
    QObject::connect(mSaveasAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    QObject::connect(mSaveallAction, SIGNAL(triggered()), this, SLOT(saveAll()));
    QObject::connect(mCloseAction, SIGNAL(triggered()), mMdi, SLOT(closeActiveSubWindow()));
    QObject::connect(mCloseallAction, SIGNAL(triggered()), this, SLOT(closeAll()));

    QObject::connect(mEditModeAction, SIGNAL(triggered()), this, SLOT(setEditMode()));
    QObject::connect(mExportConfigAction, SIGNAL(triggered()), this, SLOT(exportConfig()));

    QObject::connect(mSimulModeAction, SIGNAL(triggered()), this, SLOT(setSimulMode()));
    QObject::connect(mPlayAction, SIGNAL(triggered()), this, SLOT(play()));
    QObject::connect(mRestartAction, SIGNAL(triggered()), this, SLOT(restart()));

    QObject::connect(mTileAction, SIGNAL(triggered()), this, SLOT(tileSubwin()));
    QObject::connect(mCascadeAction, SIGNAL(triggered()), this, SLOT(cascadeSubwin()));
    QObject::connect(mTabAction, SIGNAL(triggered()), this, SLOT(tabSubwin()));
    QObject::connect(mPreviousAction, SIGNAL(triggered()), mMdi, SLOT(activatePreviousSubWindow()));
    QObject::connect(mNextAction, SIGNAL(triggered()), mMdi, SLOT(activateNextSubWindow()));

    QObject::connect(&Dispatcher::obj, SIGNAL(readyClose()), this, SLOT(close()));
    QObject::connect(&Dispatcher::obj, SIGNAL(readyCloseAll()), this, SLOT(closeAll()));
    QObject::connect(&Dispatcher::obj, SIGNAL(readyCloseActive()), mMdi, SLOT(closeActiveSubWindow()));

    //this->setUnifiedTitleAndToolBarOnMac(true);
}


// Crée les actions pour les menus.
void MainWindow::createActions()
{
    // Dossier contenant les icônes.
    QString folder(":/icons/");

    mFileMenu = this->menuBar()->addMenu("&Fichier");
    mNewAction = mFileMenu->addAction(QIcon(folder + "new.png"), "&Nouveau projet");
    mOpenAction = mFileMenu->addAction(QIcon(folder + "open.png"), "&Ouvrir...");
    mSaveAction = mFileMenu->addAction(QIcon(folder + "save.png"), "&Enregistrer");
    mSaveasAction = mFileMenu->addAction(QIcon(folder + "save_as.png"), "Enregistrer &sous...");
    mSaveallAction = mFileMenu->addAction(QIcon(folder + "save_all.png"), "&Tout enregistrer");
    mCloseAction = mFileMenu->addAction(QIcon(folder + "close.png"), "&Fermer");
    mCloseallAction = mFileMenu->addAction(QIcon(folder + "close_all.png"), "Tout fermer");
    mEditMenu = this->menuBar()->addMenu("&Edition");
    mEditModeAction = mEditMenu->addAction(QIcon(folder + "edit.png"), QString::fromUtf8("Mode &édition"));
    mExportConfigAction = mEditMenu->addAction(QIcon(folder + "export.png"), QString::fromUtf8("E&xporter la configuration"));
    mSimulMenu = this->menuBar()->addMenu("&Simulation");
    mSimulModeAction = mSimulMenu->addAction(QIcon(folder + "run.png"), "Mode &simulation");
    mPlayAction = mSimulMenu->addAction(QIcon(folder + "play.png"), QString::fromUtf8("&Démarrer"));
    mRestartAction = mSimulMenu->addAction(QIcon(folder + "restart.png"), QString::fromUtf8("&Redémarrer"));
    mWindowMenu = this->menuBar()->addMenu(QString::fromUtf8("Fe&nêtre"));
    mTileAction = new QAction(QString::fromUtf8("Réorganiser"), this);
    mCascadeAction = new QAction("Cascade", this);
    mTabAction = new QAction("Tab", this);
    mPreviousAction = new QAction(QIcon(folder + "left.png"), QString::fromUtf8("Précédent"), this);
    mNextAction = new QAction(QIcon(folder + "right.png"), "Suivant", this);
    mSeparatorAction = new QAction(this);
}


// Fermeture de la fenêtre.
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (Dispatcher::waitClose())
        mMdi->closeAllSubWindows();
    else
        event->ignore();
}


// Menu "Fichier".
void MainWindow::newFile()
{
    Document* document = createDocument();
    document->newFile();
    document->show();
}

void MainWindow::open()
{
    QString path = QFileDialog::getOpenFileName(this);
    if (!path.isEmpty())
    {
        QMdiSubWindow* existing = this->findDocument(path);
        if (existing)
        {
            mMdi->setActiveSubWindow(existing);
            return;
        }

        Document* document = createDocument();
        if (document->load(path))
        {
            statusBar()->showMessage(QString::fromUtf8("Fichier chargé"), 2000);
            document->show();
        }
        else
        {
            document->close();
            QMessageBox::critical(this, "Erreur de chargement", "Une erreur est survenue lors du chargement du fichier.");
        }
    }
}

void MainWindow::save()
{
    Document* active = activeDocument();
    if (active && active->save())
        statusBar()->showMessage(QString::fromUtf8("Fichier sauvegardé"), 2000);
}

void MainWindow::saveAs()
{
    Document* active = activeDocument();
    if (active && active->saveAs())
        statusBar()->showMessage(QString::fromUtf8("Fichier sauvegardé"), 2000);
}

void MainWindow::saveAll()
{
    for (auto& window : mMdi->subWindowList())
    {
        Document* document = qobject_cast<Document*>(window->widget());
        if (document->modified())
            document->save();
    }

    mSaveallAction->setEnabled(false);
}

void MainWindow::closeAll()
{
    if (Dispatcher::waitCloseAll())
        mMdi->closeAllSubWindows();
}


// Menu "Edition".
void MainWindow::setEditMode()
{
    Document* active = activeDocument();
    if (active)
    {
        active->setEditMode();
        this->updateActions();
    }
}

void MainWindow::exportConfig()
{
    Document* active = activeDocument();
    if (active)
    {
        Configuration config = active->config();
        if (!config.check())
        {
            QMessageBox::critical(this, "Collisions", "La configuration est invalide. Impossible de l'exporter.");
            return;
        }

        Document* document = createDocument();
        document->newFile();
        document->setConfig(config);
        document->show();
    }
}


// Menu "Simulation".
void MainWindow::setSimulMode()
{
    Document* active = activeDocument();
    if (active)
    {
        active->setSimulMode();
        this->updateActions();
    }
}

void MainWindow::play()
{
    Document* active = activeDocument();
    if (active)
    {
        mPlayAction->setText(active->playing() ? QString::fromUtf8("&Démarrer") : "&Pause");
        QString folder(":/icons/");
        mPlayAction->setIcon(QIcon(folder + (active->playing() ? "play.png" : "pause.png")));

        active->play();
    }
}

void MainWindow::restart()
{
    Document* active = activeDocument();
    if (active)
        active->restart();
}


// Menu "fenêtre".
void MainWindow::tileSubwin()
{
    mMdi->setViewMode(QMdiArea::SubWindowView);
    mMdi->tileSubWindows();
}

void MainWindow::cascadeSubwin()
{
    mMdi->setViewMode(QMdiArea::SubWindowView);
    mMdi->cascadeSubWindows();
}

void MainWindow::tabSubwin()
{
    mMdi->setViewMode(QMdiArea::TabbedView);
}


// Met à jour la barre de statut.
void MainWindow::statusText(QString text)
{
    this->statusBar()->showMessage(text);
}


// Crée un document vide.
Document* MainWindow::createDocument()
{
    Document* document = new Document;
    mMdi->addSubWindow(document);

    return document;
}

// Active un document.
void MainWindow::setActiveDocument(QWidget* window)
{
    if (!window)
        return;
    mMdi->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
}

// Recherche de documents.
Document* MainWindow::activeDocument()
{
    if (QMdiSubWindow* active = mMdi->activeSubWindow())
        return qobject_cast<Document*>(active->widget());
    return 0;
}

QMdiSubWindow* MainWindow::findDocument(const QString& path)
{
    QString canonicalPath = QFileInfo(path).canonicalFilePath();

    foreach (QMdiSubWindow* window, mMdi->subWindowList())
    {
        Document* document = qobject_cast<Document*>(window->widget());
        if (document->path() == canonicalPath)
            return window;
    }
    return 0;
}


// Mise à jour des actions disponibles.
void MainWindow::updateActions()
{
    Document* doc = this->activeDocument();
    bool hasDocument = (doc != 0);
    mCloseAction->setEnabled(hasDocument);
    mCloseallAction->setEnabled(hasDocument);
    mEditModeAction->setEnabled(hasDocument);
    mSimulModeAction->setEnabled(hasDocument);
    mTileAction->setEnabled(hasDocument);
    mCascadeAction->setEnabled(hasDocument);
    mNextAction->setEnabled(hasDocument);
    mPreviousAction->setEnabled(hasDocument);
    mSeparatorAction->setVisible(hasDocument);

    mPlayAction->setText((doc && doc->playing()) ? "&Pause" : QString::fromUtf8("&Démarrer"));
    QString folder(":/icons/");
    mPlayAction->setIcon(QIcon(folder + ((doc && doc->playing()) ? "pause.png" : "play.png")));

    bool ready = (doc && doc->ready());
    mSaveAction->setEnabled(ready);
    mSaveasAction->setEnabled(ready);
    mSaveallAction->setEnabled(ready);

    bool simul = (doc && doc->simulMode());
    mPlayAction->setEnabled(simul);
    mRestartAction->setEnabled(simul);

    mExportConfigAction->setEnabled(doc && !doc->simulMode());

    for (auto& window : mMdi->subWindowList())
    {
        Document* document = qobject_cast<Document*>(window->widget());

        if (window == mMdi->activeSubWindow())
            QObject::connect(document, SIGNAL(statusText(QString)), this, SLOT(statusText(QString)));
        else
            QObject::disconnect(document, SIGNAL(statusText(QString)), this, SLOT(statusText(QString)));
    }
}

// Mise à jour de la liste des fenêtres.
void MainWindow::updateWindowMenu()
{
    mWindowMenu->clear();
    mWindowMenu->addAction(mTileAction);
    mWindowMenu->addAction(mCascadeAction);
    mWindowMenu->addAction(mTabAction);
    mWindowMenu->addSeparator();
    mWindowMenu->addAction(mNextAction);
    mWindowMenu->addAction(mPreviousAction);
    mWindowMenu->addAction(mSeparatorAction);

    QList<QMdiSubWindow*> windows = mMdi->subWindowList();
    mSeparatorAction->setVisible(!windows.isEmpty());

    for (int i = 0 ; i < windows.size() ; ++i)
    {
        Document* document = qobject_cast<Document*>(windows[i]->widget());

        QString text;
        if (i < 9)
            text = "&%1 %2";
        else
            text = "%1 %2";
        text = text.arg(i + 1).arg(document->userFriendlyPath());

        QAction* action = mWindowMenu->addAction(text);
        action->setCheckable(true);
        action->setChecked(document == this->activeDocument());
        QObject::connect(action, SIGNAL(triggered()), mSignalMapper, SLOT(map()));
        mSignalMapper->setMapping(action, windows.at(i));
    }
}
