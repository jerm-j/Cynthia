/*
    WaveformGenerator.h

    To hold the logic for generating a number of waveforms that can be then populated into the wavetable

    // note these are all currently naive-waveform implementations
*/
#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

// a base struct that defines what an implementation will need to do.
struct WaveformGenerator
{
    virtual ~WaveformGenerator() = default;
    // perhaps make param arg "const" since its really only meant to be read-only
    virtual void fillWavetable(juce::AudioBuffer<float>& wt, const int channelNumber) = 0;
};

// refactor this
// Sine implementation
struct SineGenerator : public WaveformGenerator
{
    void fillWavetable(juce::AudioBuffer<float>& wavetable, const int channelNumber) override
    {
        auto *samples = wavetable.getWritePointer(channelNumber);
        auto tableSize = wavetable.getNumSamples();
        auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize) /*(double)(tableSize-1) which is better?*/;
        auto currentAngle = 0.0;

        for (unsigned int i = 0; i < tableSize; ++i)
        {
            auto sample = std::sin(currentAngle);
            samples[i] = (float)sample;
            currentAngle += angleDelta;
        }
    }
};

// Sawtooth implementation
struct SawtoothGenerator : public WaveformGenerator
{
    void fillWavetable(juce::AudioBuffer<float>& wavetable, const int channelNumber) override
    {
        auto *samples = wavetable.getWritePointer(channelNumber);
        auto tableSize = wavetable.getNumSamples();
        
        for(unsigned int i = 0; i < tableSize; ++i)
        {
            float t = static_cast<float>(i) / tableSize;
            samples[i] = 2.0f * t - 1.0f;
        }
    }
};

// Triangle implementation
struct TriangleGenerator : public WaveformGenerator
{
    void fillWavetable(juce::AudioBuffer<float>& wavetable, const int channelNumber) override
    {
        auto *samples = wavetable.getWritePointer(channelNumber);
        auto tableSize = wavetable.getNumSamples();

        for(unsigned int i = 0; i < tableSize; ++i)
        {
            float t = static_cast<float>(i) / tableSize;
            samples[i] = 4.0f*abs(t-0.5f)-1.0f;
        }
    }
};

// Square implementation
struct SquareGenerator : public WaveformGenerator
{
    void fillWavetable(juce::AudioBuffer<float>& wavetable, const int channelNumber) override
    {
        auto *samples = wavetable.getWritePointer(channelNumber);
        auto tableSize = wavetable.getNumSamples();

        for(unsigned int i = 0; i < tableSize; ++i)
        {
            float t = static_cast<float>(i)/tableSize;
            samples[i] = t < 0.5f ? 1.0f : -1.0f;
        }
    }
};

// custom wave generator ?
