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

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <vector>
#include <memory>

#define OUTPUT_UNLIMITED_SINKS -1

class Input;

class Output
{
public:
    Output() :
        maxSinks(-1)
    {

    }
    
    virtual ~Output()
    {

    }

    inline void setMaxSinks(int amt)
    {
        maxSinks = amt;
        if (amt >= 0)
        {
            sinks.reserve(amt);
        }
    }

    virtual inline int plug(std::shared_ptr<Input> in)
    {
        if (maxSinks >= 0 && sinks.size() >= maxSinks)
        {
            return 1;
        }

        sinks.push_back(in);

        return 0;
    }

protected:
    int maxSinks;
    std::vector<std::shared_ptr<Input>> sinks;
};

#endif /* OUTPUT_HPP */
