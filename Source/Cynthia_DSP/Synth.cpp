/*
    Synth.cpp

    Source: "Creating Synthesizer Plug-ins with C++ and JUCE" by Matthijs Hollemans
*/

#include "Cynthia_DSP/Synth.h"

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

// reset the synth's voices back to a "cleared" state
void Synth::reset()
{
    for (Voice &voice : voices)
        voice.reset();
}

void Synth::render(juce::AudioBuffer<float> &outputBuffers, int sampleCount, int bufferOffset)
{
    for (int sample = 0; sample < sampleCount; ++sample)
    {
        float output = 0.0f;

        // variable to increment num active voices
        for (Voice &voice : voices)
        {
            if (voice.env.isActive())
            {
                // increment that variable

                // need to normalize this by number of active voices
                // apply some sort of limiter, or scale by 1/MAX_VOICES
                // this is a polyphony gain staging problem
                output += voice.render();
            }
                
        }

        output = juce::jlimit(-1.0f, 1.0f, output);

        // scale the output by the number of active voices

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

// based on Matthijs Hollemans' voice-stealing logic
// Source: "Creating Synthesizer Plug-ins with C++ and JUCE"
int Synth::findFreeVoice() const
{
    int freeVoiceIndex = 0;
    float refLevel = 1.0f; // louder than any envelope

    for (int voiceIndex = 0; voiceIndex < MAX_VOICES; ++voiceIndex)
    {
        const auto& voice = voices[voiceIndex];
        float level = voice.env.getCurrentLevel();

        if (!voice.env.isActive())
        {
            return voiceIndex;
        }

        if(level < refLevel)
        {
            refLevel = level;
            freeVoiceIndex = voiceIndex;
        }
    }

    return freeVoiceIndex;
}

// starts a single voice by choosing a voice at the given index
// initializes the dsp modules
void Synth::startVoice(int voiceIndex, int note, int velocity)
{
    Voice &voice = voices[voiceIndex];

    voice.note = note;
    voice.amplitude = (velocity / 127.0f) * outputGain;
    float frequency = static_cast<float>(juce::MidiMessage::getMidiNoteInHertz(note));
    
    voice.prepareWavetable(frequency, sampleRate);
    voice.setWaveformIndicesOsc(waveformIndexAOsc, waveformIndexBOsc);
    voice.setMorphValueOsc(morphValueOsc);
    voice.setDetuneCentsOsc(detuneCentsOsc);

    voice.prepareLFO(modFreqLFO, sampleRate);
    voice.setWaveformIndicesLFO(waveformIndexALFO, waveformIndexBLFO);
    voice.setMorphValueLFO(morphValueLFO);
    voice.setDetuneCentsLFO(detuneCentsLFO);
    voice.setModDepthLFO(modDepthLFO);

    voice.prepareFilter(sampleRate);
    voice.setFilterCutoff(filterCutoff);
    voice.setFilterResonance(filterResonance);
    voice.setFilterType(filterType);

    voice.prepareEnvelope(sampleRate);
    voice.setEnvelopeParameters(envAttack, envDecay, envSustain, envRelease);
    voice.startEnvelope();
}

// dispatched from midiMessage()
void Synth::noteOn(int note, int velocity)
{
    int freeVoiceIndex = 0; // voice index 0 = mono voice

    if (numVoices > 1) // polyphony activated
    {
        freeVoiceIndex = findFreeVoice();
    }

    startVoice(freeVoiceIndex, note, velocity);
}

// dispatched from midiMessage()
void Synth::noteOff(int note)
{
    for (Voice &voice : voices)
    {
        if (voice.note == note)
        {
            voice.stopEnvelope();
            voice.note = 0;
        }
    }
}

void Synth::setOscMorphValue(float newMorphValue)
{
    morphValueOsc = juce::jlimit(0.0f, 1.0f, newMorphValue);
}

void Synth::setOscDetuneCentsValue(float newDetuneCents)
{
    detuneCentsOsc = juce::jlimit(-100.0f, 100.0f, newDetuneCents);
}

void Synth::setOscWaveformIndices(int newWaveformIndexA, int newWaveformIndexB)
{
    waveformIndexAOsc = juce::jlimit(0, 3, newWaveformIndexA);
    waveformIndexBOsc = juce::jlimit(0, 3, newWaveformIndexB);
}

void Synth::setFilterType(int newType)
{
    filterType = newType;
}

void Synth::setFilterCutoff(float newCutoff)
{
    filterCutoff = newCutoff;
}

void Synth::setFilterResonance(float newResonance)
{
    filterResonance = newResonance;
}

void Synth::setEnvAttack(float attack)
{
    envAttack = attack;
}

void Synth::setEnvDecay(float decay)
{
    envDecay = decay;
}

void Synth::setEnvSustain(float sustain)
{
    envSustain = sustain;
}
    
void Synth::setEnvRelease(float release)
{   
    envRelease = release;
}

void Synth::setLFOMorphValue(float newMorphValue)
{
    morphValueLFO = juce::jlimit(0.0f, 1.0f, newMorphValue);
}

void Synth::setLFODetuneCentsValue(float newDetuneCents)
{
    detuneCentsLFO = juce::jlimit(-100.0f, 100.0f, newDetuneCents);
}

void Synth::setLFOWaveformIndices(int newWaveformIndexA, int newWaveformIndexB)
{
    waveformIndexALFO = juce::jlimit(0, 3, newWaveformIndexA);
    waveformIndexBLFO = juce::jlimit(0, 3, newWaveformIndexB);
}

void Synth::setLFOModDepthValue(float newModDepth)
{
    modDepthLFO = juce::jlimit(0.0f, 1.0f, newModDepth);
}

void Synth::setLFOModFreqValue(float frequency) 
{
    modFreqLFO = juce::jlimit(1.0f, 500.0f,frequency);
}