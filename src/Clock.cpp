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

#include "Clock.hpp"
#include <SDL.h>

#define MILLIS SDL_GetTicks

#include <iostream>

namespace mode7
{

static int interval = 20;
static long cur = 0;
static long prev = 0;
static long elapsed = 0;
static long lag = 0;
static float m_fps;
//static float ratio = 0.5f;

void Clock::start()
{
    prev = MILLIS();
    lag = 0;
}

void Clock::tick()
{
    cur = MILLIS();
    elapsed = cur - prev;
    //std::cout << "cur: " << cur << "\telapsed: " << elapsed << std::endl;
    m_fps = 1000.0 / (float)elapsed;
    prev = cur;
    lag += elapsed;
}

void Clock::lagTick()
{
    //std::cout << lag << std::endl;
    lag -= interval;
}

long Clock::delta()
{
    return elapsed;
}

bool Clock::lagging()
{
    return lag >= interval;
}

float Clock::fps()
{
    return m_fps;
}

float Clock::extrapolate()
{
    float ex = (float)lag / (float)interval;
    if (ex < 0.0f || ex > 1.0f)
    {
        ex = 0.0f;
    }
    return ex;
}

};
