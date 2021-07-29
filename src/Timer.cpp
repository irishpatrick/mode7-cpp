/**
 * This file is part of mode7-cpp.
 * Copyright (C) 2021  Patrick Roche
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#include "Timer.hpp"

namespace mode7
{

Timer::Timer() :
    position(0),
    interval(0),
    loop(false),
    running(false)
{

}

Timer::~Timer()
{

}

void Timer::create(int inter, bool l)
{
    interval = inter;
    loop = l;
}

void Timer::start()
{
    running = true;
}

void Timer::reset()
{
    position = 0;
}

bool Timer::tick()
{
    if (!running)
    {
        return false;
    }

    ++position;

    if (position == interval)
    {
        if (loop)
        {
            reset();
        }
        else
        {
            running = false;
        }

        return true;
    }

    return false;
}

}