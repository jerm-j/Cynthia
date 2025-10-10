/*
    Synth.h

    The job of Synth is to produce the actual sound.

    In this object, we will compose within it a Vpice object which manages the oscillator.

    Source: "Creating Synthesizer Plug-ins with C++ and JUCE" by Matthijs Hollemans

*/

#pragma once

#include "Voice.h"
#include "NoiseGenerator.h"
#include "Utils.h"

class Synth
{
    public:
        Synth(const juce::AudioBuffer<float>& wavetableToUse);
        // called right before the host starts playing audio (analogous to prepareToPlay())
        void allocateResources(double sampleRate, int samplesPerBlock);
        // called right after the host has finished playing audio (analogous to releaseResources())
        void deallocateResources();
        // reset the state of the synth
        void reset();
        // render the current block of audio
        // void render(float** outputBuffers, int sampleCount);
        void render(juce::AudioBuffer<float>& outputBuffers, int sampleCount, int bufferOffset);
        // handle any midi messages
        void midiMessage(uint8_t data0, uint8_t data1, uint8_t data2);

    private:

        // handle a Note on event
        void noteOn(int note, int velocity);
        // handle a Note off event
        void noteOff(int note);

        float sampleRate;
        Voice voice;
};