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

#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <QSet>
#include <QMap>

class Document;

// Classe permettant de distribuer le temps de calcul entre des simulations simultanées.
// Cette classe permet aussi d'enregistrer les événements pour les effectuer au bon moment.
class Dispatcher : public QObject
{
    Q_OBJECT

public:
    // Un unique objet est construit pour l'application.
    static Dispatcher obj;

    // Change l'état de simulation d'un document (play ou pause).
    static void toogleDocument(Document* doc);
    // Envoie comme requête le redémarrage du document.
    static void waitRestart(Document* doc);
    // Envoie comme requête la fermeture du document et renvoie l'état de simulation du document.
    static bool waitClose(Document* doc);
    // Envoie comme requête la fermeture de tous les documents.
    static bool waitCloseAll();
    // Envoie comme requête la fermeture de la fenêtre principale.
    static bool waitClose();

signals:
    // Signaux émis lorsque les événements sont prêts à être traités.
    void readyClose();
    void readyCloseAll();
    void readyCloseActive();

private:
    // Constructeur.
    Dispatcher();

    // Lance la boucle principale de simulation.
    void run();

    // Etat global.
    bool mRunning;
    bool mSimulStep;
    bool mWaitClose;
    bool mWaitCloseAll;

    // Liste des documents dans la file d'attente.
    QSet<Document*> mSetPlay;
    QMap<Document*, bool> mWaitPlay;
    QSet<Document*> mWaitRestart;
    QSet<Document*> mWaitCloseDoc;
};

#endif // DISPATCHER_HPP
