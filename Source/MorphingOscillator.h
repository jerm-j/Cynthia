/*
    MorphingOscillator.h

    An improvement and enhancement on WavetableOscillator

    Still a wavetable based oscillator

    Key differences:
        Allows for morphing of two waveforms
        Supports detuning of each waveform
*/

#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include "WaveformGenerator.h"

class MorphingOscillator
{
public:

    MorphingOscillator()
    {
        // initialize oscillator wavetable to be populated and ready to go with waveforms
        generateWaveforms();
    }

    // prepare wavetable based on a given frequency and sample rate.
    void prepareWavetable(float frequency, float sampleRate)
    {
        this->sampleRate = sampleRate;
        baseFrequency = frequency;
        baseTableDelta = frequency * tableSize / sampleRate;

        updateDetuneFactors();
    }

    // reset oscillator phase back to beginning of wavetable
    void reset()
    {
        currentIndexA = 0.0;
        currentIndexB = 0.0;
    }

    // set the two waveforms to morph between
    void setWaveformIndices(int newWaveformIndexA, int newWaveformIndexB)
    {
        // waveformIndexA and waveformIndexB correspond to the channels of AudioBuffer
        // being used as the wavetable. Each waveform gets its own channel.
        // 0-3 maps to sine, saw, square, and triangle respectively
        waveformIndexA = juce::jlimit(0, 3, newWaveformIndexA);
        waveformIndexB = juce::jlimit(0, 3, newWaveformIndexB);
    }
    
    void setMorphValue(float newMorphValue)
    {
        // waveformA and waveformB share a single morph value.
        // 0.0 means we hear only waveformA, 1.0 means we hear only waveformB
        morphValue = juce::jlimit(0.0f, 1.0f, newMorphValue);
    }

    // set the detune amount in cents between waveformA and B
    void setDetuneCents(float newDetuneCents)
    {
        detuneCents = newDetuneCents;
        updateDetuneFactors();
    }

    // generate next output sample from the oscillator
    // this function handles morphing between two waveforms and wraps the phase increment
    float getNextSample()
    {
        float sampleA = getSampleFromWavetable(waveformIndexA, currentIndexA);
        float sampleB = getSampleFromWavetable(waveformIndexB, currentIndexB);

        // take a sample from waveformA, and waveformB and morph them together
        // if morphValue = 0.0, we only hear waveformA
        // if morphValue = 1.0, we only hear waveformB
        float output = (1.0f-morphValue) * sampleA + morphValue * sampleB;

        // increment phase idices and wrap around table size
        if ((currentIndexA += tableDeltaA) >= (float) tableSize)
            currentIndexA -= (float) tableSize;

        if ((currentIndexB += tableDeltaB) >= (float) tableSize)
            currentIndexB -= (float) tableSize;
        
        return output;
    }

private:

    // generate default waveforms and populate wavetable
    // uses WaveformGenerator interface for each waveform type
    void generateWaveforms()
    {
        // using unique_ptr to automatically manage memory allocated for generators
        // each generator fills one channel of the wavetable
        std::unique_ptr<WaveformGenerator> waveformGenerators[4] = {
            std::make_unique<SineGenerator>(),
            std::make_unique<SawtoothGenerator>(),
            std::make_unique<TriangleGenerator>(),
            std::make_unique<SquareGenerator>()
        };

        for(int waveGenIndex = 0; waveGenIndex < 4; ++waveGenIndex)
        {
            waveformGenerators[waveGenIndex]->fillWavetable(wavetable, waveGenIndex);
        }
    }

    // update phase increments for detuning between waveformA and B.
    void updateDetuneFactors()
    {
        // detuneCents value is given by detune parameter in UI
        // detuneA and detuneB correspond to waveformA and B respectively
        // we cut the detuneCents in half (because there are two wavetables)
        // then make detuneA negative so that waveformA will be detuned below the base frequency
        // then waveFormB will be above the base frequency
        float detuneA = -detuneCents * 0.5f;
        float detuneB = +detuneCents * 0.5f;

        // here we convert cent values to frequency ratios
        // one semitone = 100 cents
        // 12 semitones = 1 octave
        // 1 octave = 1200 cents
        // frequency ratio formula is 2^(cents/1200)
        float detuneFactorA = std::pow(2.0f, detuneA/1200.0f);
        float detuneFactorB = std::pow(2.0f, detuneB/1200.0f);

        // now apply the detuned frequency ratios to the base phase increment for each wavetable
        tableDeltaA = baseTableDelta * detuneFactorA;
        tableDeltaB = baseTableDelta * detuneFactorB;
    }

    // returns a single sample from the specified wavetable using linear interpolation
    float getSampleFromWavetable(int wavetableIndex, int currentIndex)
    {
        auto *table = wavetable.getReadPointer(wavetableIndex);

        int index0 = static_cast<int>(currentIndex);
        // correctly wraps the wavetable 
        int index1 = (index0+1) % tableSize;

        float frac = static_cast<float>(currentIndex - (float) index0);

        auto value0 = table[index0];
        auto value1 = table[index1];

        // linear interpolation
        auto currentSample = value0 + frac * (value1 - value0);

        return currentSample;
    }

    static constexpr int tableSize = 2048; // number of samples per waveform
    juce::AudioBuffer<float> wavetable {4, tableSize}; // can hold 4 waveforms

    double currentIndexA = 0.0; // phase index waveformA
    double currentIndexB = 0.0; // phase index waveformB
    double tableDeltaA = 0.0; // phase increment waveformA
    double tableDeltaB = 0.0; // phase increment waveformB
    double baseTableDelta = 0.0;
    float baseFrequency = 440.0f;
    float sampleRate = 44100.0f;

    int waveformIndexA = 0;
    int waveformIndexB = 1;
    float morphValue = 0.0f;
    float detuneCents = 0.0f;
    
};
