/*
    WavetableOscillator.h

    A simple Wavetable synth engine. Uses linear interpolation.

    Source: https://juce.com/tutorials/tutorial_wavetable_synth/
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class WavetableOscillator
{

public:
    WavetableOscillator(const juce::AudioBuffer<float> &waveTableToUse)
        : wavetable(waveTableToUse)
    {
    }

    void prepareWavetable(float frequency, double sampleRate)
    {
        auto tableSize = (float)wavetable.getNumSamples();
        tableDelta = frequency * tableSize / (float)sampleRate;
    }

    void reset()
    {
        currentIndex = 0;
        tableDelta = 0.0f;
    }

    // where the interpolation between wavetables occurs
    forcedinline float getNextSample() noexcept
    {
        auto tableSize = (unsigned int)wavetable.getNumSamples();

        auto index0 = (unsigned int)currentIndex;
        auto index1 = index0 == (tableSize - 1) ? (unsigned int)0 : index0 + 1;

        auto frac = currentIndex - (float)index0;

        auto *table = wavetable.getReadPointer(0);

        auto value0 = table[index0];
        auto value1 = table[index1];

        auto currentSample = value0 + frac * (value1 - value0);
        if ((currentIndex += tableDelta) > (float)tableSize)
            currentIndex -= (float)tableSize;

        return currentSample;
    }

private:
    const juce::AudioBuffer<float> &wavetable;
    float currentIndex = 0.0f; // current wavetable index
    float tableDelta = 0.0f;   // phase increment(angle delta)
};