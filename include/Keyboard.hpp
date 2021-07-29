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

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <SDL.h>
#include <string>

namespace mode7
{

class Keyboard
{
public:
    static void attach();
    static void poll();
    static bool isDown(SDL_Scancode);
    static bool isDown(const std::string&);
    static bool isPressed(SDL_Scancode);
    static bool isPressed(const std::string&);

    static int lock();
    static void unlock();
    static bool available(int);
};

}

#endif /* KEYBOARD_HPP */