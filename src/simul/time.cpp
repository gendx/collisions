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

#include "time.hpp"

// Affichage dans un flux standard.
std::ostream& operator<<(std::ostream& flux, const Time& time)
{
    if (time.mNever)
        flux << "never";
    else
        flux << time.mTime;
    return flux;
}


// Constructeurs.
Time::Time() :
    mTime(0),
    mNever(true)
{
}

Time::Time(double time) :
    mTime(time >= 0 ? time : 0),
    mNever(time >= 0 ? false : true)
{
}

Time::Time(const Time& time) :
    mTime(time.mTime),
    mNever(time.mNever)
{
}


// Opérateurs.
void Time::operator=(const Time& time)
{
    mTime = time.mTime;
    mNever = time.mNever;
}

Time Time::operator-(const Time& time) const
{
    Time result(*this);
    result -= time;
    return result;
}

void Time::operator-=(const Time& time)
{
    if (mNever || time.mNever || mTime < time.mTime)
    {
        mNever = true;
        mTime = 0;
    }
    else
        mTime -= time.mTime;
}

Time Time::operator+(const Time& time) const
{
    Time result(*this);
    result += time;
    return result;
}

void Time::operator+=(const Time& time)
{
    if (mNever || time.mNever)
    {
        mNever = true;
        mTime = 0;
    }
    else
        mTime += time.mTime;
}


// Comparaison.
bool Time::operator<(const Time& time) const
{
    return mNever ? false : (time.mNever ? true : mTime < time.mTime);
}

bool Time::operator==(const Time& time) const
{
    return (mNever == time.mNever
            ? (mNever
               ? true
               : mTime == time.mTime)
            : false);
}
