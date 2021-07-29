/**
 * This file is part of mode7-cpp.
 * Copyright (C) 2021  Patrick Roche
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

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
    return paContinue;
}

Master::Master() : 
    Input(),
    Output(),

    m_sampleRate(44100),
    m_framesPerBuffer(128),
    m_maxChannels(64),
    m_numChannels(0),
    m_soundData(),
    m_stream(nullptr),
    m_pdi(nullptr),
    m_output_params()
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

    err = Pa_OpenStream(
        &m_stream, 
        NULL, 
        &m_output_params, 
        m_sampleRate, 
        m_framesPerBuffer, 
        paClipOff, 
        pa_callback, 
        nullptr
    );
    if (err != paNoError)
    {
        shutdown();
        return 1;
    }

    err = Pa_StartStream(m_stream);
    if (err != paNoDevice)
    {
        shutdown();
        return 1;
    }
}

int Master::stop()
{
    PaError err;

    err = Pa_StopStream(m_stream);
    if (err != paNoError)
    {
        shutdown();
        return 1;
    }

    err = Pa_CloseStream(m_stream);
    if (err != paNoError)
    {
        shutdown();
        return 1;
    }

    return 0;
}

void Master::shutdown()
{
    Pa_Terminate();
}
