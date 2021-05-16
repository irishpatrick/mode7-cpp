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
        m_pos += 
            m_states.top().leftRate * (m_dir < 0 && m_pos <= 0) + 
            m_states.top().rightRate * (m_dir > 0 && m_pos >= 0) + 
            -m_states.top().restoreRate * (m_dir < 0 && m_pos >= 0) + 
            m_states.top().restoreRate * (m_dir > 0 && m_pos <= 0) + 
            (m_dir == 0) * (1.f * m_states.top().restoreRate * (m_pos < 0.f) + 
            -1.f * m_states.top().restoreRate * (m_pos > 0.f));
        
        if (m_pos > 1.f)
        {
            m_pos = 1.f;
        }
        else if (m_pos < -1.f)
        {
            m_pos = -1.f;
        }
        
        if (fabs(m_pos) < 1e-2)
        {
            m_pos = 0.f;
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
