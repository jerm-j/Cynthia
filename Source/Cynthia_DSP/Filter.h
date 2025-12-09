/*
    Filter.h

    Includes an abstract base class, Filter, to allow implementations of different filter types in the future

    Currently one implementation: SVFilter (internally JUCE's StateVariableTPTFilter)

*/


#pragma once

#include <juce_dsp/juce_dsp.h>

class Filter
{
public:
    virtual ~Filter() = default;

    virtual void prepare(double sampleRate) = 0;
    virtual void reset() = 0;

    virtual void setCutoff(float cutoffHz) = 0;
    virtual void setResonance(float q) = 0;
    virtual float processSample(float input) = 0;
};


class SVFFilter : public Filter
{
public:

    enum class Mode 
    {
        LowPass, HighPass, BandPass
    };

    void prepare(double newSampleRate) override
    {
        sampleRate = newSampleRate;
        spec.sampleRate = newSampleRate;
        spec.maximumBlockSize = 1;
        spec.numChannels = 1;
        filter.prepare(spec);
        updateFilterType();
    }

    void reset() override
    {
        filter.reset();
    }

    void setCutoff(float cutoffHz) override
    {
        filter.setCutoffFrequency(cutoffHz);
    }

    void setResonance(float q) override
    {
        filter.setResonance(q);
    }

    // sets the mode member variable "mode" and then calls 
    // updateFilterType(), which updates the internal JUCE filter's mode
    void setMode(int newMode)
    {
        switch(newMode)
        {
            case 0: 
                mode = Mode::LowPass; 
                break;
            case 1: 
                mode = Mode::HighPass; 
                break;
            case 2: 
                mode = Mode::BandPass; 
                break;
        }

        updateFilterType();
    }

    float processSample(float sample) override
    {
        // mono signal
        return filter.processSample(0, sample);
    }

private:

    void updateFilterType()
    {
        using FilterType = juce::dsp::StateVariableTPTFilterType;

        switch(mode)
        {
            case Mode::LowPass:
                filter.setType(FilterType::lowpass);
                break;

            case Mode::HighPass:
                filter.setType(FilterType::highpass);
                break;

            case Mode::BandPass:
                filter.setType(FilterType::bandpass);
                break;
        }
    }

    juce::dsp::StateVariableTPTFilter<float> filter;
    Mode mode = Mode::LowPass;
    double sampleRate = 44100.0;
    juce::dsp::ProcessSpec spec;
};

