#ifndef CONTROLSLIDER_HPP
#define CONTROLSLIDER_HPP

namespace mode7
{

class ControlSlider
{
public:
    ControlSlider();
    ~ControlSlider();

    void setAutoUp(bool);
    void setDownRate(float);
    void setUpRate(float);

    float getPosition();

    void down();
    void up();  
    void update();

private:
    float m_pos;
    bool m_autoUp;
    int m_dir;
    float m_downRate;
    float m_upRate;
};

}

#endif /* CONTROLSLIDER_HPP */