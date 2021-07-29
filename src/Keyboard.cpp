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

#include "Keyboard.hpp"
#include <cstdint>
#include <cstring>

namespace mode7
{

static uint8_t states[1000];
static const uint8_t* keys;
static int lockVal = 0;

void Keyboard::attach()
{
    memset(&states, 0, sizeof(states));
}

void Keyboard::poll()
{
    keys = SDL_GetKeyboardState(nullptr);
}

bool Keyboard::isDown(SDL_Scancode sc)
{
    return keys[sc];
}

bool Keyboard::isDown(const std::string& key)
{
    return isDown(SDL_GetScancodeFromName(key.c_str()));
}

bool Keyboard::isPressed(SDL_Scancode sc)
{
    if (keys[sc] && !states[sc])
    {
        states[sc] = 1;
        return true;
    }
    else if (keys[sc] && states[sc])
    {
        return false;
    }
    else if (!keys[sc] && states[sc])
    {
        states[sc] = 0;
        return false;
    }
    return false;
}

bool Keyboard::isPressed(const std::string& key)
{
    return isPressed(SDL_GetScancodeFromName(key.c_str()));
}

int Keyboard::lock()
{
    return ++lockVal;
}

void Keyboard::unlock()
{
    if (lockVal > 0)
    {
        --lockVal;
    }
}

bool Keyboard::available(int val)
{
    return lockVal == val;
}

}
