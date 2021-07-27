#include "WaveformGenerator.hpp"

#include <cmath>
#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE 200

WaveformGenerator::WaveformGenerator() :
    Output()
{
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        m_sine_table.push_back(
            (float)sin(((double)i / (double)TABLE_SIZE) * M_PI * 2.));
    }
}

WaveformGenerator::~WaveformGenerator()
{

}
