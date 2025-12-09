/*
    Voice.h

    Voice is an object that manages a single playing note.

    The job of Voice is to produce the next output sample for a given note.

    In monophonic mode, there will be only one note playing at a time, 
    and therefore Synth just needs one Voice instance.

    In polyphonic mode, there will be a Voice instance for every note that is playing.

    Therefore, each note is associated with a voice, and each voice has its own oscillator.
    
    Source: "Creating Synthesizer Plug-ins with C++ and JUCE" by Matthijs Hollemans
*/

#pragma once

#include "Cynthia_DSP/MorphingOscillator.h"
#include "Cynthia_DSP/MorphingLFO.h"
#include "Cynthia_DSP/Envelope.h"
#include "Cynthia_DSP/Filter.h"

struct Voice
{
    int note;
    MorphingOscillator osc;
    MorphingLFO lfo;
    Envelope env;
    SVFFilter filter;
    float amplitude;

    Voice() 
    {}

    // reset the voice back to a "cleared" state
    void reset()
    {
        note = 0;
        osc.reset();
        env.reset();
        filter.reset();
        lfo.resetPhase();
    }

    float render()
    {
        float sample = osc.getNextSample();
        float envelope = env.getNextSample();   
        float filtered_sample = filter.processSample(sample);

        float lfoAmount = lfo.getNextLFOSample();
        float amplitudeModulator = 1.0f + lfoAmount;
        amplitudeModulator = juce::jlimit(-1.0f, 1.0f, amplitudeModulator);

        // our signal chain
        return filtered_sample * envelope * amplitude * amplitudeModulator;
        
        /*
            little trick for debugging the envelope or lfo: 

                return envelope;
                return lfoAmount;

            this will let us view output through oscilloscope
        */
        
    }

    void prepareWavetable(float frequency, float sampleRate)
    {
        osc.prepareWavetable(frequency, sampleRate);
    }

    void setWaveformIndicesOsc(int newWaveformIndexA, int newWaveformIndexB)
    {
        osc.setWaveformIndices(newWaveformIndexA, newWaveformIndexB);
    }

    void setMorphValueOsc(float newMorphValue)
    {
        osc.setMorphValue(newMorphValue);
    }

    void setDetuneCentsOsc(float newDetuneCents)
    {
        osc.setDetuneCents(newDetuneCents);
    }

    void prepareFilter(float sampleRate)
    {
        filter.prepare(sampleRate);
    }

    void setFilterType(int newType)
    {
        filter.setMode(newType);
    }

    void setFilterCutoff(float newCutoff)
    {
        filter.setCutoff(newCutoff);
    }

    void setFilterResonance(float newResonance)
    {
        filter.setResonance(newResonance);
    }

    void prepareEnvelope(float sampleRate)
    {
        env.prepare(sampleRate);
    }

    void setEnvelopeParameters(float attack, float decay, float sustain, float release)
    {
        env.setParameters(attack, decay, sustain, release);
    }

    void startEnvelope()
    {
        env.noteOn();
    }

    void stopEnvelope()
    {
        env.noteOff();
    }

    void prepareLFO(float lfoRate, float sampleRate)
    {
        lfo.prepareLFO(lfoRate, sampleRate);
    }

    void setWaveformIndicesLFO(int newWaveformIndexA, int newWaveformIndexB)
    {
        lfo.setWaveformIndices(newWaveformIndexA, newWaveformIndexB);
    }

    void setMorphValueLFO(float newMorphValue)
    {
        lfo.setMorphValue(newMorphValue);
    }

    void setDetuneCentsLFO(float newDetuneCents)
    {
        lfo.setDetuneCents(newDetuneCents);
    }

    void setModDepthLFO(float newModDepth)
    {
        lfo.setModDepth(newModDepth);
    }

};