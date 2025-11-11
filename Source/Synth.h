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
        Synth();
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

        void setMorphValue(float newMorphValue);

        void setWaveformIndices(int newWaveformIndexA, int newWaveformIndexB);
        
        void setDetuneCentsValue(float newDetuneCents);

        float outputGain;

        float envAttack;
        float envDecay;
        float envSustain;
        float envRelease;

        int filterType;
        float filterCutoff;
        float filterResonance;

        static constexpr int MAX_VOICES = 8;
        int numVoices;

        int waveformIndexA = 0;
        int waveformIndexB = 1;
        float morphValue = 0.0f;
        float detuneCents = 0.0f;

    private:

        // handle the triggering of a voice
        void startVoice(int voiceIndex, int note, int velocity);

        // part of the voice stealing logic
        int findFreeVoice() const;

        // handle a Note on event
        void noteOn(int note, int velocity);
        // handle a Note off event
        void noteOff(int note);

        float sampleRate;

        // this allocates room for all 8 voices. In monophonic mode, the synth
        // will only use the first object: voices[0]
        std::array<Voice, MAX_VOICES> voices;
};