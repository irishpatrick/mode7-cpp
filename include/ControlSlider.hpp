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
    void setSpeed(float);

    float getPosition();

    void down();
    void up();  
    void update();
};

}

#endif /* CONTROLSLIDER_HPP */