/*
    Synth.cpp

    Source: "Creating Synthesizer Plug-ins with C++ and JUCE" by Matthijs Hollemans
*/

#include "Synth.h"

Synth::Synth()
{
    sampleRate = 44100.0f;
}

void Synth::allocateResources(double sampleRate, int /*samplesPerBlock*/)
{
    this->sampleRate = static_cast<float>(sampleRate);
}

void Synth::deallocateResources()
{
    // need to implement
}

// reset the synth's voice back to a "cleared" state
void Synth::reset()
{
    for (Voice &voice : voices)
        voice.reset();
}

void Synth::render(juce::AudioBuffer<float> &outputBuffers, int sampleCount, int bufferOffset)
{
    // this only renders the voice if the envelope is still active.
    /*
        For future me:
            knowing when a voice is no longer used is important for polyphony
            and voice management.

            The voice stealing logic will look at which voices are not currently playing and it uses
            the envelope's active state for this.
    */

    for (int sample = 0; sample < sampleCount; ++sample)
    {
        float output = 0.0f;

        for (Voice &voice : voices)
        {
            if (voice.env.isActive())
                output += voice.render();
        }

        for (int channel = 0; channel < outputBuffers.getNumChannels(); ++channel)
        {
            outputBuffers.addSample(channel, sample + bufferOffset, output);
        }
    }

    for (Voice &voice : voices)
    {
        if (!voice.env.isActive())
            voice.env.reset();
    }
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
        if (velocity > 0)
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

void Synth::setMorphValue(float newMorphValue)
{
    morphValue = juce::jlimit(0.0f, 1.0f, newMorphValue);
}

void Synth::setDetuneCentsValue(float newDetuneCents)
{
    detuneCents = juce::jlimit(-100.0f, 100.0f, newDetuneCents);
}

void Synth::setWaveformIndices(int newWaveformIndexA, int newWaveformIndexB)
{
    waveformIndexA = juce::jlimit(0, 3, newWaveformIndexA);
    waveformIndexB = juce::jlimit(0, 3, newWaveformIndexB);
}

int Synth::findFreeVoice() const
{
    int freeVoiceIndex = 0;
    float refLevel = 100.0f; // louder than any envelope

    for (int voiceIndex = 0; voiceIndex < MAX_VOICES; ++voiceIndex)
    {
        if (voices[voiceIndex].env.level < refLevel && !voices[voiceIndex].env.isInAttack())
        {
            refLevel = voices[voiceIndex].env.level;
            freeVoiceIndex = voiceIndex;
        }
    }

    return freeVoiceIndex;
}

void Synth::startVoice(int voiceIndex, int note, int velocity)
{
    Voice &voice = voices[voiceIndex];

    voice.note = note;
    voice.amplitude = (velocity / 127.0f) * outputGain;
    auto frequency = juce::MidiMessage::getMidiNoteInHertz(note);
    
    voice.osc.prepareWavetable(static_cast<float>(frequency), sampleRate);
    voice.setWaveformIndices(waveformIndexA, waveformIndexB);
    voice.setMorphValue(morphValue);
    voice.setDetuneCents(detuneCents);

    voice.filter.prepare(sampleRate);
    voice.filter.setCutoff(filterCutoff);
    voice.filter.setResonance(filterResonance);
    voice.filter.setMode(filterType);

    Envelope &env = voice.env;
    env.attackMultiplier = envAttack;
    env.decayMultiplier = envDecay;
    env.sustainLevel = envSustain;
    env.releaseMultiplier = envRelease;
    env.attack();
}

void Synth::noteOn(int note, int velocity)
{
    int freeVoiceIndex = 0; // voice index 0 = mono voice

    if (numVoices > 1) // polyphony activated
    {
        freeVoiceIndex = findFreeVoice();
    }

    startVoice(freeVoiceIndex, note, velocity);
}

void Synth::noteOff(int note)
{
    for (Voice &voice : voices)
    {
        if (voice.note == note)
        {
            voice.release();
            voice.note = 0;
        }
    }
}