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

#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include "graph.hpp"
#include "configuration.hpp"
#include "simulateur.hpp"
#include "editeur.hpp"
#include "dispatcher.hpp"

// Widget représentant un document associé à une configuration de simulation.
class Document : public QWidget
{
    Q_OBJECT

    friend class Dispatcher;

public:
    // Constructeur.
    Document();

    // Fichiers de configuration.
    void newFile();
    bool load(const QString& path);
    bool save();
    bool saveAs();
    bool setConfig(const Configuration& config);

    // Accesseurs.
    inline QString userFriendlyPath();
    inline QString path();

    inline bool modified();
    inline bool ready();
    inline bool playing();
    inline bool simulMode();

    inline Configuration config();

    // Change l'état de simulation.
    void setEditMode();
    void setSimulMode();

    void restart();
    void play();

signals:
    // Statut à afficher dans une QStatusBar.
    void statusText(QString);

private slots:
    // Requêtes de dessin.
    void resizeGraph(int, int);
    void draw();
    void fullDraw();

private:
    // Gestion des événements.
    void closeEvent(QCloseEvent* event);
    void resizeEvent(QResizeEvent* = NULL);
    void showEvent(QShowEvent*);

    // Chemin du fichier associé.
    void setPath(const QString& path);
    inline QString strippedName(const QString& fullPath);

    // Enregistrement du fichier.
    bool maybeSave();
    bool save(const QString& path);

    // Etat du document.
    QString mPath;
    bool mUntitled;
    bool mModified;
    bool mReady;
    bool mSimulMode;
    bool mPlaying;

    // Widgets.
    QVBoxLayout* mLayout;
    QSplitter* mSplitter;
    Graph* mGraph;
    QStackedWidget* mStackedWidget;
    Simulateur* mSimulateur;
    Editeur* mEditeur;

    // Configuration associée.
    Configuration mConfig;
};


// Accesseurs.
inline bool Document::modified()
    {return mModified;}
inline bool Document::ready()
    {return mReady;}
inline bool Document::playing()
    {return mPlaying;}
inline bool Document::simulMode()
    {return mSimulMode;}

inline Configuration Document::config()
    {return mEditeur->config();}

// Chemin du fichier associé.
inline QString Document::strippedName(const QString& fullPath)
    {return QFileInfo(fullPath).fileName();}
inline QString Document::userFriendlyPath()
    {return this->strippedName(mPath);}
inline QString Document::path()
    {return mPath;}

#endif // DOCUMENT_HPP
