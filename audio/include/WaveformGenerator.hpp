#ifndef WAVEFORMGENERATOR_HPP
#define WAVEFORMGENERATOR_HPP

#include "Output.hpp"

#include <vector>

class WaveformGenerator : public Output
{
public:
    WaveformGenerator();
    virtual ~WaveformGenerator();

private:
    std::vector<float> m_sine_table;
    std::vector<float> m_triangle_table;
    std::vector<float> m_square_table;
    std::vector<float> m_sawtooth_table;
};

#endif /* WAVEFORMGENERATOR_HPP */
