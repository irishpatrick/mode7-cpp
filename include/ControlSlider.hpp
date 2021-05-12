#ifndef CONTROLSLIDER_HPP
#define CONTROLSLIDER_HPP

namespace mode7
{

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

private:
    float m_pos;
    bool m_autoUp;
    bool m_restoring;
    int m_dir;
    float m_downRate;
    float m_upRate;
    float m_leftRate;
    float m_rightRate;
    float m_restoreRate;
};

}

#endif /* CONTROLSLIDER_HPP */