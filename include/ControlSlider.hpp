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

#ifndef CONTROLSLIDER_HPP
#define CONTROLSLIDER_HPP

#include <stack>

namespace mode7
{

typedef struct _ControlSliderState
{
    bool autoUp;
    bool restoring;
    float downRate;
    float upRate;
    float leftRate;
    float rightRate;
    float restoreRate;
} ControlSliderState;

class ControlSlider
{
public:
    ControlSlider();
    ~ControlSlider();

    void restoreToCenter(bool);
    void setAutoUp(bool);
    void setDownRate(float);
    void setUpRate(float);
    void setRestoreRates(float, float, float);

    float getPosition();

    void down();
    void up();
    void left();
    void right();

    void update();

    void pushCurrentState();
    void popCurrentState();
    unsigned int getSp();

private:
    std::stack<ControlSliderState> m_states;
    float m_pos;
    int m_dir;
};

}

#endif /* CONTROLSLIDER_HPP */