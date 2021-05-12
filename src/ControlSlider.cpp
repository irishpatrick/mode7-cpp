#include "ControlSlider.hpp"

#include <cmath>

namespace mode7
{

ControlSlider::ControlSlider() :
    m_pos(0.f)
{

}

ControlSlider::~ControlSlider()
{

}

void ControlSlider::down()
{
    if (m_restoring) return;
    m_dir = 1;
}

void ControlSlider::up()
{
    if (m_restoring) return;
    m_dir = -1;
}

void ControlSlider::left()
{
    if (!m_restoring) return;
    m_dir = -1;
}

void ControlSlider::right()
{
    if (!m_restoring) return;
    m_dir = 1;
}

float ControlSlider::getPosition()
{
    return m_pos;
}

void ControlSlider::update()
{
    if (m_restoring)
    {
        m_pos += m_leftRate * (m_dir < 0) + m_rightRate * (m_dir > 0) + (m_dir == 0) * (1.f * m_restoreRate * (m_pos < 0.f) + -1.f * m_restoreRate * (m_pos > 0.f));
        if (m_pos > 1.f) m_pos = 1.f;
        else if (m_pos < -1.f) m_pos = -1.f;
        if (fabs(m_pos) < 1e-2)
        {
            m_pos = 0.f;
        }
        m_dir = 0;
    }
    else
    {
        m_pos += m_downRate * (m_dir > 0) + m_upRate * (m_dir < 0);

        if (m_pos < 0.f)
        {
            m_pos = 0.f;
        }

        if (m_pos > 1.f)
        {
            m_pos = 1.f;
        }

        m_dir = 0;

        if (m_autoUp)
        {
            up();
        }
    }
}

void ControlSlider::setAutoUp(bool val)
{
    m_autoUp = val;
}

void ControlSlider::setDownRate(float rate)
{
    m_downRate = rate;
}

void ControlSlider::setUpRate(float rate)
{
    m_upRate = -rate;
}

void ControlSlider::setRestoreRates(float left, float right, float rest)
{
    m_restoring = true;
    m_leftRate = -left;
    m_rightRate = right;
    m_restoreRate = rest;
}

} // namespace mode7
