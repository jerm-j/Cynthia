#pragma once
#include "Cynthia_DSP/Synth.h"
#include "Cynthia_DSP/WaveformGenerator.h"
#include "Cynthia_Utilities/Utils.h"

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
    void createWaveTable(std::shared_ptr<juce::AudioBuffer<float>> wt); // called only once at initilization
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) override
    {
        parametersChanged.store(true);
    }

    std::atomic<bool> parametersChanged { false };

    void update();
    void updatePolyMode();
    void updateADSR();
    void updateFilter();
    void updateDateWavetable();
    void updateLFO();
    
    Synth synth;

    /*
        Note that these param variables are pointers.
        The actual parameter objects are owned by the AudioProcessorValueTreeState.
        Meaning we will need to initialize them in the constructor.
    */
    juce::AudioParameterChoice* wavetypeAParamOsc;
    juce::AudioParameterChoice* wavetypeBParamOsc;
    juce::AudioParameterFloat* morphValueParamOsc;
    juce::AudioParameterFloat* detuneCentsParamOsc;

    juce::AudioParameterChoice* wavetypeAParamLFO;
    juce::AudioParameterChoice* wavetypeBParamLFO;
    juce::AudioParameterFloat* morphValueParamLFO;
    juce::AudioParameterFloat* detuneCentsParamLFO;
    juce::AudioParameterFloat* modDepthParamLFO;
    juce::AudioParameterFloat* modFreqParamLFO;

    juce::AudioParameterChoice* polyModeParam;
    juce::AudioParameterFloat* envAttackParam;
    juce::AudioParameterFloat* envDecayParam;
    juce::AudioParameterFloat* envSustainParam;
    juce::AudioParameterFloat* envReleaseParam;

    juce::AudioParameterChoice* filterTypeParam;
    juce::AudioParameterFloat* filterCutoffParam;
    juce::AudioParameterFloat* filterResonanceParam;

    juce::AudioParameterFloat* outputGainParam;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CynthiaAudioProcessor)
};
