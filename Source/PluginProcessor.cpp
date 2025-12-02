#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CynthiaAudioProcessor::CynthiaAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
{
    castParameter(apvts, ParameterID::wavetypeAOsc, wavetypeAParamOsc);
    castParameter(apvts, ParameterID::wavetypeBOsc, wavetypeBParamOsc);
    castParameter(apvts, ParameterID::morphValueOsc, morphValueParamOsc);
    castParameter(apvts, ParameterID::detuneCentsOsc, detuneCentsParamOsc);

    castParameter(apvts, ParameterID::wavetypeALFO, wavetypeAParamLFO);
    castParameter(apvts, ParameterID::wavetypeBLFO, wavetypeBParamLFO);
    castParameter(apvts, ParameterID::morphValueLFO, morphValueParamLFO);
    castParameter(apvts, ParameterID::detuneCentsLFO, detuneCentsParamLFO);
    castParameter(apvts, ParameterID::modDepthLFO, modDepthParamLFO);
    castParameter(apvts, ParameterID::modFreqLFO, modFreqParamLFO);

    castParameter(apvts, ParameterID::polyMode, polyModeParam);

    castParameter(apvts, ParameterID::envAttack, envAttackParam);
    castParameter(apvts, ParameterID::envDecay, envDecayParam);
    castParameter(apvts, ParameterID::envSustain, envSustainParam);
    castParameter(apvts, ParameterID::envRelease, envReleaseParam);

    castParameter(apvts, ParameterID::filterType, filterTypeParam);
    castParameter(apvts, ParameterID::filterCutoff, filterCutoffParam);
    castParameter(apvts, ParameterID::filterResonance, filterResonanceParam);

    castParameter(apvts, ParameterID::outputGain, outputGainParam);

    apvts.state.addListener(this);
}

CynthiaAudioProcessor::~CynthiaAudioProcessor()
{
    apvts.state.removeListener(this);
}

// a method for any pre-playback intialization
void CynthiaAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.allocateResources(sampleRate, samplesPerBlock);
    parametersChanged.store(true);
    reset();
}

// when playback stops, you can use this method as an opportunity to free up any spare memory, etc.
void CynthiaAudioProcessor::releaseResources()
{
    synth.deallocateResources();
}

// reset the audio processor's state upon any changes to the synth object
void CynthiaAudioProcessor::reset()
{
    synth.reset();
}

void CynthiaAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                         juce::MidiBuffer &midiMessages)
{
    /*
        Denormals are floating point numbers that are so small, they are practically 0.
        Denormals are slower than regular floating point numbers, so we treat them as 0.

        juce::ScopedNoDenormals sets a CPU flag for this method to truncate floating point numbers
        to 0 instead of turning them into denormal numbers.
    */
    juce::ScopedNoDenormals noDenormals;

    /*
        JUCE does not guarantee the AudioBuffer is already cleared.
        So we must clear it of potential garbage values.
        This is to prevent, in the worst case, screaming feedback!
    */

    buffer.clear();

    bool expected = true;
    
    // this line does a thread-safe check to see if parametersChanged is true.
    // if so, it calls the update method to perform parameter recalculation
    // then immediately sets parametersChanged back to false
    if (parametersChanged.compare_exchange_strong(expected, false))
        update();

    splitBufferByEvents(buffer, midiMessages);
}

//==============================================================================

// Source: "Creating Synthesizer Plug-ins with C++ and JUCE" by Matthijs Hollemans
void CynthiaAudioProcessor::splitBufferByEvents(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    int bufferOffset = 0;

    for (const auto metadata : midiMessages)
    {
        // Render the audio that happens before this event (if any)
        int samplesThisSegment = metadata.samplePosition - bufferOffset;
        if (samplesThisSegment > 0)
        {
            render(buffer, samplesThisSegment, bufferOffset);
            bufferOffset += samplesThisSegment;
        }

        // Handle the event. Ignore midi messages such as sysex.
        if (metadata.numBytes <= 3)
        {
            uint8_t data1 = (metadata.numBytes >= 2) ? metadata.data[1] : 0;
            uint8_t data2 = (metadata.numBytes == 3) ? metadata.data[2] : 0;
            handleMIDI(metadata.data[0], data1, data2);
        }
    }

    /*
        Render the audio after the last MIDI event. If there were no
        MIDI events at all, this renders the entire buffer.
    */
    int samplesLastSegment = buffer.getNumSamples() - bufferOffset;
    if (samplesLastSegment > 0)
    {
        render(buffer, samplesLastSegment, bufferOffset);
    }
    midiMessages.clear();
}

// Source: "Creating Synthesizer Plug-ins with C++ and JUCE" by Matthijs Hollemans
void CynthiaAudioProcessor::handleMIDI(uint8_t data0, uint8_t data1, uint8_t data2)
{
    // send the MIDI data to our Synth object to be turned into sound
    synth.midiMessage(data0, data1, data2);
}

// Source: "Creating Synthesizer Plug-ins with C++ and JUCE" by Matthijs Hollemans
void CynthiaAudioProcessor::render(juce::AudioBuffer<float> &buffer, int sampleCount, int bufferOffset)
{
    synth.render(buffer, sampleCount, bufferOffset);
}

// inside this method we will instantiate all the parameter objects
juce::AudioProcessorValueTreeState::ParameterLayout CynthiaAudioProcessor::createParameterLayout()
{
    // a helper object used to construct the APVTS
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    return configureLayout();
}

void CynthiaAudioProcessor::update()
{
    synth.outputGain = outputGainParam->get();
    
    updatePolyMode();

    updateFilter();

    updateADSR();

    updateDateWavetable();

    updateLFO();
}

void CynthiaAudioProcessor::updateDateWavetable() 
{

    synth.setOscWaveformIndices(wavetypeAParamOsc->getIndex(), wavetypeBParamOsc->getIndex());
    synth.setOscMorphValue(morphValueParamOsc->get());
    synth.setOscDetuneCentsValue(detuneCentsParamOsc->get());
}

void CynthiaAudioProcessor::updateLFO()
{
    synth.setLFOWaveformIndices(wavetypeAParamLFO->getIndex(), wavetypeBParamLFO->getIndex());
    synth.setLFOMorphValue(morphValueParamLFO->get());
    synth.setLFODetuneCentsValue(detuneCentsParamLFO->get());
    synth.setLFOModDepthValue(modDepthParamLFO->get());
    synth.setLFOModFreqValue(modFreqParamLFO->get());
}

void CynthiaAudioProcessor::updatePolyMode()
{
    synth.numVoices = (polyModeParam->getIndex() == 0) ? 1 : Synth::MAX_VOICES;
}

void CynthiaAudioProcessor::updateFilter()
{
    synth.filterType = filterTypeParam->getIndex();
    synth.filterCutoff = filterCutoffParam->get();
    synth.filterResonance = filterResonanceParam->get();
}

void CynthiaAudioProcessor::updateADSR()
{
    synth.envAttack = envAttackParam->get();
    synth.envDecay = envDecayParam->get();
    synth.envSustain = envSustainParam->get();
    synth.envRelease = envReleaseParam->get();  
}

//==============================================================================
const juce::String CynthiaAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CynthiaAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool CynthiaAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool CynthiaAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double CynthiaAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CynthiaAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int CynthiaAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CynthiaAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String CynthiaAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void CynthiaAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================

// Called by the DAW to query the number of channels supported
bool CynthiaAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

//==============================================================================
bool CynthiaAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *CynthiaAudioProcessor::createEditor()
{
    return new CynthiaAudioProcessorEditor(*this);
}

//==============================================================================
void CynthiaAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void CynthiaAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new CynthiaAudioProcessor();
}
