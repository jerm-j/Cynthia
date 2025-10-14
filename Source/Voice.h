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


struct Voice
{
    int note;
    WavetableOscillator osc;

    Voice(const juce::AudioBuffer<float>& wavetableToUse) 
        : osc(wavetableToUse)
    {
    }

    // reset the voice back to a "cleared" state
    void reset()
    {
        note = 0;
        osc.reset();
    }

    float render()
    {
        return osc.getNextSample();
    }

};