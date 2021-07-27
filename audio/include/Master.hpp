#ifndef MASTER_HPP
#define MASTER_HPP

#include "Input.hpp"
#include "Output.hpp"

#include <portaudio.h>

#define TABLE_SIZE 200

typedef struct _SoundData
{
    float buffer[TABLE_SIZE];
} SoundData;

class Master : public Input, Output
{
public:
    Master();
    ~Master();

    int init();
    int stop();
    void shutdown();

private:
    int m_sampleRate;
    int m_framesPerBuffer;
    int m_maxChannels;
    int m_numChannels;
    SoundData m_soundData;
    PaStream* m_stream;
    const PaDeviceInfo* m_pdi;
    PaStreamParameters m_output_params;
};

#endif /* MASTER_HPP */
