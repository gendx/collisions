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

#include "document.hpp"

#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QPainter>
#include "configuration.hpp"

// Constructeur.
Document::Document() :
    mUntitled(true),
    mModified(false),
    mReady(false),
    mSimulMode(true),
    mPlaying(false),
    mConfig(),
    mLayout(new QVBoxLayout(this)),
    mSplitter(new QSplitter),
    mGraph(new Graph(mConfig)),
    mStackedWidget(new QStackedWidget),
    mSimulateur(new Simulateur(mConfig)),
    mEditeur(new Editeur(mGraph))
{
    // Le widget est détruit à sa fermeture.
    this->setAttribute(Qt::WA_DeleteOnClose);

    // Création de l'interface graphique.
    mSplitter->addWidget(mGraph);
    mSplitter->addWidget(mStackedWidget);

    mStackedWidget->addWidget(mSimulateur);
    mStackedWidget->addWidget(mEditeur);

    mLayout->setMargin(5);
    mLayout->addWidget(mSplitter);

    // Connexion des signaux et slots.
    QObject::connect(mSplitter, SIGNAL(splitterMoved(int, int)), this, SLOT(resizeGraph(int, int)));
    QObject::connect(mSimulateur, SIGNAL(draw()), this, SLOT(draw()));
    QObject::connect(mSimulateur, SIGNAL(fullDraw()), this, SLOT(fullDraw()));
    QObject::connect(mSimulateur, SIGNAL(statusText(QString)), this, SIGNAL(statusText(QString)));
    QObject::connect(mEditeur, SIGNAL(draw()), this, SLOT(draw()));
    QObject::connect(mEditeur, SIGNAL(fullDraw()), this, SLOT(fullDraw()));
    QObject::connect(mEditeur, SIGNAL(statusText(QString)), this, SIGNAL(statusText(QString)));
    QObject::connect(mGraph, SIGNAL(draw()), this, SLOT(draw()));
    QObject::connect(mGraph, SIGNAL(fullDraw()), this, SLOT(fullDraw()));

    // Zoom initial.
    mGraph->setZoom(0);
}


// Evénement de fermeture du widget.
void Document::closeEvent(QCloseEvent* event)
{
    // Demande la fermeture au dispatcher.
    if (mPlaying && !Dispatcher::waitClose(this))
    {
        // Un signal sera envoyé plus tard par le dispatcher pour indiquer que la fermeture est possible.
        event->ignore();
        return;
    }

    // Enregistre les modifications si nécessaire.
    if (maybeSave())
        event->accept();
    else
        event->ignore();
}

// Adaptation du dessin à la nouvelle taille.
void Document::resizeEvent(QResizeEvent*)
{
    this->draw();
}

// Redessine le widget en cas d'affichage (sinon bug).
void Document::showEvent(QShowEvent*)
{
    this->draw();
}

// Redimensionnement du graphe.
void Document::resizeGraph(int, int)
{
    this->draw();
}


// Passage au mode édition.
void Document::setEditMode()
{
    // Met la simulation en pause.
    if (mPlaying)
        Dispatcher::toogleDocument(this);

    // Affiche le widget d'édition.
    mSimulMode = false;
    mStackedWidget->setCurrentWidget(mEditeur);
    mGraph->setEditMode();
    this->draw();
}

// Passage au mode simulation.
void Document::setSimulMode()
{
    // Affiche le widget de simulation.
    mSimulMode = true;
    mStackedWidget->setCurrentWidget(mSimulateur);
    mGraph->setSimulMode();
    this->draw();
}


// Redémarre la simulation.
void Document::restart()
{
    Dispatcher::waitRestart(this);
}

// Bouton "play/pause".
void Document::play()
{
    Dispatcher::toogleDocument(this);
}


// Change la configuration du document.
bool Document::setConfig(const Configuration& config)
{
    // Passe au mode simulation.
    this->setSimulMode();

    mConfig = config;
    mEditeur->setConfig(mConfig);

    // Initialise la simulation.
    this->restart();
    mReady = true;
    return true;
}


// Associe un chemin de fichier au document.
void Document::setPath(const QString& path)
{
    mPath = QFileInfo(path).canonicalFilePath();
    mUntitled = false;
    this->setWindowModified(false);
    this->setWindowTitle(this->userFriendlyPath() + "[*]");
}

// Demande à l'utilisateur s'il veut enregistrer la configuration.
bool Document::maybeSave()
{
    if (mModified)
    {
        QMessageBox::StandardButton result = QMessageBox::warning(this, "Collisions", QString::fromUtf8("Le fichier '%1' a été modifié.\nVoulez vous le sauvegarder?").arg(this->userFriendlyPath()), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (result == QMessageBox::Save)
            return this->save();
        else if (result == QMessageBox::Cancel)
            return false;
    }
    return true;
}


// Crée un document vide en mode édition.
void Document::newFile()
{
    static int id = 0;

    mUntitled = true;
    mPath = QString("document%1.col").arg(++id);
    this->setWindowTitle(mPath + "[*]");

    this->setEditMode();
}

// Enregistre la configuration.
bool Document::save()
{
    if (mUntitled)
        return this->saveAs();
    else
        return this->save(mPath);
}

// Choisit un fichier et sauvegarde la configuration.
bool Document::saveAs()
{
    QString path = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", "", "Fichiers collisions (*.col)");
    if (path.isNull())
        return false;

    return this->save(path);
}

// Enregistre dans un fichier.
bool Document::save(const QString& path)
{
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "Erreur d'enregistrement", QString("Impossible d'enregistrer le fichier %1:\n%2").arg(path).arg(file.errorString()));
        return false;
    }

    // Ecrit le fichier.
    QDataStream stream(&file);
    stream << quint32(0xC0117870) << mConfig;

    // Change le chemin associé au document.
    this->setPath(path);
    return true;
}

// Charge un fichier.
bool Document::load(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    // Vérifie l'en-tête magique.
    QDataStream stream(&file);
    quint32 magic;
    stream >> magic;
    if (magic != 0xC0117870)
        return false;

    // Charge la configuration.
    Configuration config;
    stream >> config;
    file.close();

    // S'il y a des erreurs, on arrête.
    if (stream.status() != QDataStream::Ok)
        return false;

    // Change la configuration du document.
    mConfig = config;
    mEditeur->setConfig(mConfig);

    this->restart();
    mReady = true;
    this->setPath(path);
    return true;
}


// Redessine l'arrière plan et les particules.
void Document::fullDraw()
{
    mGraph->reinitBackground(true);
    this->draw();
}

// Redessine les particules.
void Document::draw()
{
    // Obtension de l'arrière plan.
    mGraph->reinitBackground(false);
    QPixmap pixmap(mGraph->getBackground());
    QPainter painter(&pixmap);
    mGraph->initPainter(painter);

    // Affichage selon le mode.
    if (mSimulMode)
        mSimulateur->draw(painter, pixmap.width());
    else
        mEditeur->draw(painter);

    // Envoie l'image au système de fenêtrage.
    mGraph->setPixmap(pixmap);
    mGraph->update();
}
