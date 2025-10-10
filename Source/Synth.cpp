/*
    Synth.cpp

    Source: "Creating Synthesizer Plug-ins with C++ and JUCE" by Matthijs Hollemans
*/

#include "Synth.h"

Synth::Synth(const juce::AudioBuffer<float>& wavetableToUse) 
    : voice(wavetableToUse)
{
    sampleRate = 44100.0f;
}

void Synth::allocateResources(double _sampleRate, int /*samplesPerBlock*/)
{
    sampleRate = static_cast<float>(_sampleRate);
}

void Synth::deallocateResources()
{
    // need to implement
}

// reset the synth's voice back to a "cleared" state
void Synth::reset()
{
    voice.reset();
}

void Synth::render(juce::AudioBuffer<float>& outputBuffers, int sampleCount, int bufferOffset)
{
    float *outputBufferLeft = outputBuffers.getWritePointer(0) + bufferOffset;
    float *outputBufferRight = outputBuffers.getWritePointer(1) + bufferOffset;

    for(int sample = 0; sample < sampleCount; ++sample)
    {
        float output = 0.0f;

        if(voice.note > 0)
        {
            output = voice.render();
        }

        *outputBufferLeft++ = output;
        if(outputBufferRight)
        {
            *outputBufferRight++ = output;
        }
    }

    // protectYourEars(&outputBufferLeft, sampleCount);
    // protectYourEars(&outputBufferRight, sampleCount);
}

void Synth::midiMessage(uint8_t data0, uint8_t data1, uint8_t data2)
{
    /*
        The first byte of a MIDI msg is the status byte which consists of two parts: the command(four highest bits), and the channel number(four lowest bits).
        By doing (data0 & 0xF0), we are only looking at the status byte's command.

        If we wanted to find the channel this message applies to, we'd do the following: uint8_t channel = data0 & 0x0F
            
        The bitwise-AND operations are a defensive programming strategy to ensure that the values
        we send to noteOff and noteOn are within the range of vald MIDI numbers (0 to 127)
    */
    switch (data0 & 0xF0)
    {
        // Note off
        case 0x80:
            noteOff(data1 & 0x7F);
            break;
           
        // Note on
        case 0x90:
        {
            uint8_t note = data1 & 0x7F;
            uint8_t velocity = data2 & 0x7F;

            /* 
                A noteOn event with a velocity of 0 should be treated as a noteOff event.
                This is because of the running status feature which can omit the status byte of a midi message.
            */
            if(velocity > 0)
            {
                noteOn(note, velocity);
            }
            else
            {
                noteOff(note);
            }

            break;
        }
    }
}

void Synth::noteOn(int note, int velocity)
{
    voice.note = note;
    voice.osc.prepareWavetable(juce::MidiMessage::getMidiNoteInHertz(note), sampleRate);
}

void Synth::noteOff(int note)
{
    /*
        If voice.note is the same as the note of the key that was released, 
        we clear the voice's note and velocity fields.

        0 means no note is playing.
        0 means no velocity.
    */

    /* 
        This logic is currently causing pops and clicks on noteOff.
        Will need to implement an ADSR envelope to allow the sound ramp down smoothly.
    */
    if(voice.note == note)
    {
        voice.note = 0;
    }
}