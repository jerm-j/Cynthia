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

#include "WavetableOscillator.h"
#include "Envelope.h"

struct Voice
{
    int note;
    WavetableOscillator osc;
    Envelope env;
    float amplitude;

    Voice() {}

    // reset the voice back to a "cleared" state
    void reset()
    {
        note = 0;
        osc.reset();
        env.reset();
    }

    float render()
    {
        float sample = osc.getNextSample();
        float envelope = env.nextValue();   
        return sample * envelope * amplitude;
        
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

    // dynamically set the wavetable being used
    void setWaveTable(std::shared_ptr<juce::AudioBuffer<float>> wt)
    {
        osc.setWaveTable(wt);
    }

};