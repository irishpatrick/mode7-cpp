#include "ControlSlider.hpp"

namespace mode7
{

ControlSlider::ControlSlider()
{

}

ControlSlider::~ControlSlider()
{

}

void ControlSlider::down()
{
    m_dir = 1;
}

void ControlSlider::up()
{
    m_dir = -1;
}

void ControlSlider::update()
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

} // namespace mode7
