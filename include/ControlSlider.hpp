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