#include "Master.hpp"

#include <cmath>

static int pa_callback(
    const void* inputBuffer,
    void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    return 0;
}

Master::Master() : 
    m_sampleRate(44100),
    m_framesPerBuffer(128),
    m_maxChannels(64),
    m_numChannels(0)
{
    setMaxSinks(1);
}

Master::~Master()
{

}

int Master::init()
{
    PaError err;

    m_output_params.device = Pa_GetDefaultOutputDevice();
    if (m_output_params.device == paNoDevice)
    {
        return paInvalidDevice;
    }

    m_pdi = Pa_GetDeviceInfo(m_output_params.device);
    m_output_params.channelCount = m_pdi->maxOutputChannels;
    m_output_params.sampleFormat = paFloat32;
    m_output_params.suggestedLatency = m_pdi->defaultLowOutputLatency;
    m_output_params.hostApiSpecificStreamInfo = nullptr;

    err = Pa_OpenStream(&m_stream, NULL, &m_output_params, m_sampleRate, m_framesPerBuffer, paClipOff, pa_callback, nullptr);
}