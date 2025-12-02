#pragma once

#include "Cynthia_DSP/MorphingOscillator.h"

class MorphingLFO : public MorphingOscillator
{
public:

    MorphingLFO() = default;

    void prepareLFO(float frequency, float sampleRate)
    {
        this->sampleRate = sampleRate;
        baseFrequency = frequency;
        baseTableDelta = frequency * tableSize / sampleRate;

        updateDetuneFactors();
        resetPhase();
    }

    void setModDepth(float newDepthValue)
    {
        modDepth = juce::jlimit(0.0f, 1.0f, newDepthValue);
    }

    float getNextLFOSample()
    {
        return getNextSample() * modDepth;
    }

    void resetPhase()
    {
        reset();
    }

private:
    float modDepth = 0.0f; // scales the modulation frequency
};