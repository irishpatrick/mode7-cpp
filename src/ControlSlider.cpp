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

#include "ControlSlider.hpp"

#include <cmath>
#include <cassert>
#include <cstring>

namespace mode7
{

ControlSlider::ControlSlider() :
    m_pos(0.f),
    m_dir(0)
{
    ControlSliderState css;
    memset(&css, 0, sizeof(ControlSliderState));
    m_states.push(css);
}

ControlSlider::~ControlSlider()
{

}

void ControlSlider::down()
{
    assert(m_states.size() > 0);
    if (m_states.top().restoring) return;
    m_dir = 1;
}

void ControlSlider::up()
{
    assert(m_states.size() > 0);
    if (m_states.top().restoring) return;
    m_dir = -1;
}

void ControlSlider::left()
{
    assert(m_states.size() > 0);
    if (!m_states.top().restoring) return;
    m_dir = -1;
}

void ControlSlider::right()
{
    assert(m_states.size() > 0);
    if (!m_states.top().restoring) return;
    m_dir = 1;
}

float ControlSlider::getPosition()
{
    return m_pos;
}

void ControlSlider::update()
{
    assert(m_states.size() > 0);
    if (m_states.top().restoring)
    {
        float damping = fmax(0.01, 0.15 * log(fabs(m_pos)) + 1.0); // prevent position from bouncing with high restore coefficient
        m_pos += 
            m_states.top().leftRate * (m_dir < 0 && m_pos <= 0) + // wheels left turning left
            m_states.top().rightRate * (m_dir > 0 && m_pos >= 0) + // wheels right turning right
            damping * -m_states.top().restoreRate * (m_dir < 0 && m_pos >= 0) + // wheels right turning left
            damping * m_states.top().restoreRate * (m_dir > 0 && m_pos <= 0) + // wheels left turing right
            (m_dir == 0) * (damping * m_states.top().restoreRate * (m_pos < 0.f) + // no steering input
            -damping * m_states.top().restoreRate * (m_pos > 0.0));
        
        if (m_pos > 1.0)
        {
            m_pos = 1.0;
        }
        else if (m_pos < -1.0)
        {
            m_pos = -1.0;
        }
        
        if (fabs(m_pos) < 4e-2)
        {
            m_pos = 0.0;
        }
        m_dir = 0;
    }
    else
    {
        m_pos += 
            m_states.top().downRate * (m_dir > 0) + 
            m_states.top().upRate * (m_dir < 0);

        if (m_pos < 0.f)
        {
            m_pos = 0.f;
        }

        if (m_pos > 1.f)
        {
            m_pos = 1.f;
        }

        m_dir = 0;

        if (m_states.top().autoUp)
        {
            up();
        }
    }
}

void ControlSlider::setAutoUp(bool val)
{
    assert(m_states.size() > 0);
    m_states.top().autoUp = val;
}

void ControlSlider::setDownRate(float rate)
{
    assert(m_states.size() > 0);
    m_states.top().downRate = rate;
}

void ControlSlider::setUpRate(float rate)
{
    assert(m_states.size() > 0);
    m_states.top().upRate = -rate;
}

void ControlSlider::setRestoreRates(float left, float right, float rest)
{
    assert(m_states.size() > 0);
    m_states.top().restoring = true;
    m_states.top().leftRate = -left;
    m_states.top().rightRate = right;
    m_states.top().restoreRate = rest;
}

void ControlSlider::pushCurrentState()
{
    assert(m_states.size() > 0);
    ControlSliderState css;
    css = m_states.top();
    m_states.push(css);
}

void ControlSlider::popCurrentState()
{
    if (m_states.size() <= 1)
    {
        return;
    }

    m_states.pop();
}

unsigned int ControlSlider::getSp()
{
    return m_states.size() - 1;
}

} // namespace mode7
