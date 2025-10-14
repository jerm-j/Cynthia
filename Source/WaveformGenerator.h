/*
    WaveformGenerator.h

    To hold the logic for generating a number of waveforms that can be then populated into the wavetable
*/
#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

// a base struct that defines what an implementation will need to do.
struct WaveformGenerator
{
    virtual ~WaveformGenerator() = default;
    virtual void fillWavetable(juce::AudioBuffer<float> &wavetable) = 0;
};

// Sine implementation
struct SineGenerator : public WaveformGenerator
{
    void fillWavetable(juce::AudioBuffer<float> &wavetable) override
    {
        auto *samples = wavetable.getWritePointer(0);
        auto tableSize = wavetable.getNumSamples();
        auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1);
        auto currentAngle = 0.0;

        for (unsigned int i = 0; i < tableSize; ++i)
        {
            auto sample = std::sin(currentAngle);
            samples[i] = (float)sample;
            currentAngle += angleDelta;
        }
    }
};
