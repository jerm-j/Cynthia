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

#include "MorphingOscillator.h"
#include "Envelope.h"
#include "Filter.h"

struct Voice
{
    int note;
    MorphingOscillator osc;
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
    }

    float render()
    {
        float sample = osc.getNextSample();
        float envelope = env.nextValue();   
        float filtered_sample = filter.processSample(sample);
        return filtered_sample * envelope * amplitude;
        
        /*
            little trick for debugging the envelope:
                just return the envelope and view the output through the oscilloscope
        */
        // return envelope;
    }

    void release()
    {
        env.release();
    }

    void setWaveformIndices(int newWaveformIndexA, int newWaveformIndexB)
    {
        osc.setWaveformIndices(newWaveformIndexA, newWaveformIndexB);
    }

    void setMorphValue(float newMorphValue)
    {
        osc.setMorphValue(newMorphValue);
    }

    void setDetuneCents(float newDetuneCents)
    {
        osc.setDetuneCents(newDetuneCents);
    }

};