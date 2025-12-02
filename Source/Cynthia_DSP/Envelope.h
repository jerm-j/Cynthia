#pragma once
#include <juce_dsp/juce_dsp.h>

class Envelope
{
public:
    Envelope() {};

    void prepare(float sampleRate, float samplesPerBLock)
    {
        adsr.setSampleRate(sampleRate);
    }

    void reset()
    {
        adsr.reset();
    }

    void setParameters(float attack, float decay, float sustain, float release)
    {
        params.attack = attack;
        params.decay = decay;
        params.sustain = sustain;
        params.release = release;

        adsr.setParameters(params);
    }

    float getAttack()
    {
        return params.attack;
    }

    float getSustain()
    {
        return params.sustain;
    }

    float getRelease()
    {
        return params.release;
    }

    float getCurrentLevel() const
    {
        return currentLevel;
    }

    void noteOn() 
    {
        adsr.noteOn();
    }

    void noteOff()
    {
        adsr.noteOff();
    }

    float getNextSample()
    {
        currentLevel = adsr.getNextSample();
        return currentLevel;
    }

    bool isActive() const
    {
        return adsr.isActive();
    }

private:
    juce::ADSR adsr;
    juce::ADSR::Parameters params;
    float currentLevel;
};