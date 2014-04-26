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

#include "courbes_group.hpp"

#include <QWheelEvent>

// Constructeur.
CourbesGroup::CourbesGroup(Time lifespan) :
    mLayout(new QVBoxLayout),
    mSplitter(new QSplitter(Qt::Vertical)),
    mScrollBar(new QScrollBar(Qt::Horizontal)),
    mLifespan(lifespan)
{
    // Barre de défilement.
    mScrollBar->setMinimum(0);
    mScrollBar->setMaximum(0);
    mScrollBar->setPageStep(10000);
    mScrollBar->setSingleStep(100);

    // Création de l'interface graphique.
    mLayout->setDirection(QBoxLayout::BottomToTop);
    mLayout->setContentsMargins(QMargins());
    mLayout->addWidget(mScrollBar);
    mLayout->addWidget(mSplitter);
    this->setLayout(mLayout);

    // Connexion des signaux et slots.
    QObject::connect(mScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scroll(int)));
}


// Suppression de toutes les courbes.
void CourbesGroup::clear()
{
    mCourbes.clear();
    mProfils.clear();
    mBegin = Time();
    mEnd = Time();
    mScrollBar->setMaximum(0);

    // Arrête la récupération de valeurs.
    for (int i = 0 ; i < mCourbes.size() ; ++i)
        mCourbes[i]->setFinished();
    for (int i = 0 ; i < mProfils.size() ; ++i)
        mProfils[i]->setFinished();

    // Supprime le widget.
    mLayout->removeWidget(mSplitter);
    delete mSplitter;
    mSplitter = new QSplitter(Qt::Vertical);
    mLayout->addWidget(mSplitter);
}

// Ajout d'une courbe.
void CourbesGroup::addCourbe(const ConfigWidgetCourbe& courbe)
{
    mCourbes.push_back(std::shared_ptr<WidgetCourbe>(new WidgetCourbe(mLifespan, courbe)));
    mSplitter->addWidget(mCourbes.back().get());
}

// Ajout d'un profil.
void CourbesGroup::addProfil(const ConfigProfil& profil)
{
    mProfils.push_back(std::shared_ptr<WidgetProfil>(new WidgetProfil(mLifespan, profil)));
    mSplitter->addWidget(mProfils.back().get());
}


// Ajout de valeurs aux courbes.
void CourbesGroup::push(Time time, const QList<Population>& populations, const QList<std::shared_ptr<Piston> >& pistons)
{
    for (int i = 0 ; i < mCourbes.size() ; ++i)
        mCourbes[i]->push(time, populations, pistons);
    for (int i = 0 ; i < mProfils.size() ; ++i)
        mProfils[i]->push(time, populations);

    if (time < mBegin)
        mBegin = time;
    if (mEnd.isNever() || mEnd < time)
        mEnd = time;
}

// Met à jour la barre de défilement.
void CourbesGroup::update()
{
    if (mCourbes.isEmpty() && mProfils.isEmpty())
        return;

    if (mLifespan < mEnd - mBegin)
    {
        bool max = mScrollBar->value() == mScrollBar->maximum();
        mScrollBar->setMaximum(10000.0 * ((mEnd - mBegin).time() / mLifespan.time() - 1.0));

        if (max)
            mScrollBar->setValue(mScrollBar->maximum());
    }

    this->scroll(mScrollBar->value());
}


// Défilement horizontal.
void CourbesGroup::scroll(int value)
{
    for (int i = 0 ; i < mCourbes.size() ; ++i)
    {
        mCourbes[i]->setScroll((mScrollBar->maximum() - value) / 10000.0);
        mCourbes[i]->setLifespan(mLifespan);
        mCourbes[i]->update();
    }
    for (int i = 0 ; i < mProfils.size() ; ++i)
    {
        mProfils[i]->setScroll((mScrollBar->maximum() - value) / 10000.0);
        mProfils[i]->setLifespan(mLifespan);
        mProfils[i]->update();
    }
}

// Molette de la souris.
void CourbesGroup::wheelEvent(QWheelEvent* event)
{
    if (mCourbes.isEmpty() && mProfils.isEmpty())
        return;

    double facteur = exp(-event->delta() / 1600.0);
    mLifespan = mLifespan.time() * facteur;

    // Limites de la barre de défilement.
    if (mLifespan < mEnd - mBegin)
    {
        bool max = mScrollBar->value() == mScrollBar->maximum();
        mScrollBar->setMaximum(10000.0 * ((mEnd - mBegin).time() / mLifespan.time() - 1.0));

        if (max)
            mScrollBar->setValue(mScrollBar->maximum());
        else
            mScrollBar->setValue(((mScrollBar->value() / 10000.0 + .5) / facteur - .5) * 10000.0);
    }
    else
        mScrollBar->setMaximum(0);

    this->scroll(mScrollBar->value());
}
