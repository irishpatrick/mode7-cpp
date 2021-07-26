#ifndef MASTER_HPP
#define MASTER_HPP

#include "Input.hpp"
#include "Output.hpp"

#include <portaudio.h>

class Master : public Input, Output
{
public:
    Master();
    ~Master();

    int init();

private:
    int m_sampleRate;
    int m_framesPerBuffer;
    int m_maxChannels;
    int m_numChannels;
    PaStream* m_stream;
    PaStreamParameters m_output_params;
    const PaDeviceInfo* m_pdi;
};

#endif /* MASTER_HPP */