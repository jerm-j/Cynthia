#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "Synth.h"
#include "WaveformGenerator.h"
#include "Utils.h"

/*
    Here we create a new namespace which contains juce::ParameterID objects for each parameter definition.
    To get the identifier of a parameter, we can just write "ParameterID::paramID".
    The actual string value is just "paramID".
*/
namespace ParameterID
{
    #define PARAMETER_ID(str) const juce::ParameterID str(#str, 1);
        PARAMETER_ID(wavetype)
        PARAMETER_ID(envAttack)
        PARAMETER_ID(envDecay)
        PARAMETER_ID(envSustain)
        PARAMETER_ID(envRelease)
    #undef PARAMETER_ID
}

//==============================================================================
class CynthiaAudioProcessor final : public juce::AudioProcessor, 
                                    private juce::ValueTree::Listener
{
public:
    //==============================================================================
    CynthiaAudioProcessor();
    ~CynthiaAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void reset() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    // needs to be public so the plugin editor can access it
    juce::AudioProcessorValueTreeState apvts {*this, nullptr, "Parameters", createParameterLayout()};
private:
    //==============================================================================
    void splitBufferByEvents(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages);
    void handleMIDI(uint8_t data0, uint8_t data1, uint8_t data2);
    void render(juce::AudioBuffer<float> &buffer, int sampleCount, int bufferOffset);
    void createWaveTable(); // called only once at initilization
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) override
    {
        parametersChanged.store(true);
    }

    std::atomic<bool> parametersChanged { false };

    void update();
    void updateWavetable();

    Synth synth;
    juce::AudioBuffer<float> wavetable;

    /*
        Note that these param variables are pointers.
        The actual parameter objects are owned by the AudioProcessorValueTreeState.
        Meaning we will need to initialize them in the constructor.
    */
    juce::AudioParameterChoice* wavetypeParam;

    juce::AudioParameterFloat* envAttackParam;
    juce::AudioParameterFloat* envDecayParam;
    juce::AudioParameterFloat* envSustainParam;
    juce::AudioParameterFloat* envReleaseParam;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CynthiaAudioProcessor)
};
