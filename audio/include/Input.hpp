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

#ifndef INPUT_HPP
#define INPUT_HPP

#include <vector>
#include <memory>

#define INPUT_UNLIMITED_SOURCES -1

class Output;

class Input
{
public:
    Input() :
        maxSources(-1)
    {

    }

    virtual ~Input()
    {

    }

    inline void setMaxSources(int amt)
    {
        maxSources = amt;
        if (amt >= 0)
        {
            sources.reserve(amt);
        }
    }

    virtual inline int plug(std::shared_ptr<Output> out)
    {
        if (maxSources >= 0 && sources.size() >= maxSources)
        {
            return 1;
        }

        sources.push_back(out);

        return 0;
    }

protected:
    int maxSources;
    std::vector<std::shared_ptr<Output>> sources;
};

#endif /* INPUT_HPP */
