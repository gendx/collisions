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

#include "dispatcher.hpp"

#include "document.hpp"

// Un unique objet est construit pour l'application.
Dispatcher Dispatcher::obj;

// Constructeur.
Dispatcher::Dispatcher() :
    mRunning(false),
    mSimulStep(false),
    mWaitClose(false),
    mWaitCloseAll(false)
{
}


// Change l'état de simulation d'un document (play ou pause).
void Dispatcher::toogleDocument(Document* doc)
{
    // Une simulation est en cours.
    if (obj.mSimulStep)
        // On ajoute à la file d'attente.
        obj.mWaitPlay[doc] = !doc->mPlaying;
    else
    {
        // On traite directement la requête.
        doc->mPlaying = !doc->mPlaying;
        if (doc->mPlaying)
        {
            obj.mSetPlay.insert(doc);
            obj.run();
        }
        else
            obj.mSetPlay.remove(doc);
    }
}

// Envoie comme requête le redémarrage du document.
void Dispatcher::waitRestart(Document* doc)
{
    // Une simulation est en cours.
    if (obj.mSimulStep)
        // On ajoute à la file d'attente.
        obj.mWaitRestart.insert(doc);
    else
        // On traite directement la requête.
        doc->mSimulateur->doRestart();
}

// Envoie comme requête la fermeture du document et renvoie l'état de simulation du document.
bool Dispatcher::waitClose(Document* doc)
{
    // Une simulation est en cours.
    if (obj.mSimulStep)
        // On ajoute à la file d'attente.
        obj.mWaitCloseDoc.insert(doc);
    else
    {
        // On traite directement la requête.
        doc->mPlaying = false;
        obj.mSetPlay.remove(doc);
    }

    return !doc->mPlaying;
}

// Envoie comme requête la fermeture de tous les documents.
bool Dispatcher::waitCloseAll()
{
    // Une simulation est en cours.
    if (obj.mSimulStep || obj.mWaitCloseAll)
        // On ajoute à la file d'attente.
        obj.mWaitCloseAll = true;
    else
    {
        // On traite directement la requête.
        for (auto& it : obj.mSetPlay)
            it->mPlaying = false;
        obj.mSetPlay.clear();
    }

    return !obj.mWaitCloseAll;
}

// Envoie comme requête la fermeture de la fenêtre principale.
bool Dispatcher::waitClose()
{
    // Une simulation est en cours.
    if (obj.mSimulStep || obj.mWaitClose)
        // On ajoute à la file d'attente.
        obj.mWaitClose = true;
    else
    {
        // On traite directement la requête.
        for (auto& it : obj.mSetPlay)
            it->mPlaying = false;
        obj.mSetPlay.clear();
    }

    return !obj.mWaitClose;
}

void Dispatcher::run()
{
    // Si on est déjà dans la boucle, on sort.
    if (mRunning)
        return;

    // Indique que l'on entre dans la boucle principale.
    mRunning = true;

    // Tant qu'il y a des choses à simuler.
    while (!(mWaitClose || (mSetPlay.isEmpty() && mWaitPlay.isEmpty() && mWaitRestart.isEmpty() && mWaitCloseDoc.empty())))
    {
        // Redémarre les simulations souhaitées.
        for (auto& it : mWaitRestart)
            it->mSimulateur->doRestart();
        mWaitRestart.clear();

        // Met à jour la liste des simulations en cours.
        for (auto it = mWaitPlay.begin() ; it != mWaitPlay.end() ; ++it)
        {
            if (it.value())
                mSetPlay.insert(it.key());
            else
                mSetPlay.remove(it.key());
            it.key()->mPlaying = it.value();
        }
        mWaitPlay.clear();

        // Ferme les documents souhaités.
        for (auto& it : mWaitCloseDoc)
        {
            it->mPlaying = false;
            mSetPlay.remove(it);
        }

        if (mWaitCloseAll)
        {
            mWaitCloseDoc.clear();
            mWaitCloseAll = false;
            emit readyCloseAll();
        }
        else if (!mWaitCloseDoc.empty())
        {
            mWaitCloseDoc.clear();
            emit readyCloseActive();
        }

        // Avance chaque simulation jusqu'au prochain dessin.
        mSimulStep = true;
        for (auto& it : mSetPlay)
            it->mSimulateur->playToNextDraw();
        mSimulStep = false;
    }

    // Ferme la fenêtre principale.
    if (mWaitClose)
    {
        for (auto& it : mSetPlay)
            it->mPlaying = false;
        mSetPlay.clear();

        mWaitClose = false;
        emit readyClose();
    }

    // On indique que l'on sort de la boucle principale.
    mRunning = false;
}
