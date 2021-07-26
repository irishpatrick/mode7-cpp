#ifndef MIXER_HPP
#define MIXER_HPP

#include "Input.hpp"
#include "Output.hpp"

class Mixer : public Input, Output
{
public:
    Mixer();
    ~Mixer();
};

#endif /* MIXER_HPP */