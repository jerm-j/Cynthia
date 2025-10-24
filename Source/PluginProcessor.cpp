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
                         ),
      synth(wavetable)
{
    createWaveTable();
    // wavetypeParam = dynamic_cast<juce::AudioParameterChoice*>(
    //     apvts.getParameter(ParameterID::wavetype.getParamID())
    // );
    castParameter(apvts, ParameterID::wavetype, wavetypeParam);
    castParameter(apvts, ParameterID::envAttack, envAttackParam);
    castParameter(apvts, ParameterID::envDecay, envDecayParam);
    castParameter(apvts, ParameterID::envSustain, envSustainParam);
    castParameter(apvts, ParameterID::envRelease, envReleaseParam);

    apvts.state.addListener(this);
}

CynthiaAudioProcessor::~CynthiaAudioProcessor()
{
    apvts.state.removeListener(this);
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
    if(parametersChanged.compare_exchange_strong(expected, false)) update();

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

// Source: https://juce.com/tutorials/tutorial_wavetable_synth/
// Currently, this function only generates a sine wave, and stores it into the wavetable.
/*
    A dilemma:

        If we are going to have a dynamic wavetable, meaning that the user will be able
        to select different waveforms to use on the fly (Sine, Saw, Tri..), is this
        the best way to populate the wavetable? How will we repopulate the wavetable
        with new samples during runtime? Where will this method be called from to repopulate it? Currently we're calling it from
        this class's constructor, but that only works for a one time initialization. We don't want to call it from
        prepareToPlay() either because it will be called too often and unnecessarily...

        So where should I call the waveform update logic to update the wavetable during runtime?

    Update: 

        This function is fine for initialization. The default wavetype is sine, so we will initialize
        the wavetable to be filled with samples of one period of a sine wave.

        But for dynamic reinitilization at runtime, we will need an updateWavetable() method that will
        read any changes in the UI, and refill the wavetable with samples of the new wavetype.

        */
void CynthiaAudioProcessor::createWaveTable()
{
    const unsigned int tableSize = 1 << 7; // value of 128
    wavetable.setSize(1, (int)tableSize);

    SineGenerator sineTable;
    sineTable.fillWavetable(wavetable);
}

// inside this method we will instantiate all the parameter objects
juce::AudioProcessorValueTreeState::ParameterLayout CynthiaAudioProcessor::createParameterLayout()
{
    // a helper object used to construct the APVTS
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // the ParameterLayout assumes ownership of the AudioParameter object, which is why 
    // the parameter is constructed using std::make_unique
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        ParameterID::wavetype, // the identifier
        "Wavetype", // human readable name of the parameter (this is what the DAW shows to the user)
        juce::StringArray {"Sine", "Sawtooth", "Triangle", "Square"}, // the list of wavetypes to choose from
        0 // the default choice (Sine)
    ));

    /*
        Note on Envelope ADSR params:
            Attack, Decay, Sustain, and Release are measured as percentages.
            0% = fast as possible
            100% = slow as possible

            "Sustain is a percentage of the amplitude level... it is not a time
            but a level. It determines how load the sound is during the steady part...
            Sustain level does not change based on the note velocity... no matter how loud
            or how quiet it is, the sustain level is always relative to the note's amplitude."
    */
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::envAttack,
        "Env Attack",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::envDecay,
        "Env Decay",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        50.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::envSustain,
        "Env Sustain",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        100.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::envRelease,
        "Env Release",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        30.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")
    ));

    return layout;
}

void CynthiaAudioProcessor::update()
{
    float sampleRate = float(getSampleRate());
    float inverseSampleRate = 1.0f / sampleRate;

    synth.envAttack = std::exp(-inverseSampleRate * std::exp(5.5f-0.075f * envAttackParam->get()));
    synth.envDecay = std::exp(-inverseSampleRate * std::exp(5.5f-0.075f * envDecayParam->get()));
    synth.envSustain = envSustainParam->get() / 100.0f;

    float envRelease = envReleaseParam->get();
    if(envRelease < 1.0f)
    {
        synth.envRelease = 0.075f; // extra fast release
    }
    else
    {
        synth.envRelease = std::exp(-inverseSampleRate * std::exp(5.5f-0.075f * envRelease));
    }
    
    // =======================================================

    switch(wavetypeParam->getIndex())
    {
        case 0:
        {
            wavetable.clear();
            SineGenerator sine;
            sine.fillWavetable(wavetable);
            break;
        }
            
        case 1:
        {
            wavetable.clear();
            SawtoothGenerator sawtooth;
            sawtooth.fillWavetable(wavetable);
            break;
        }
            
        case 2:
        {
            wavetable.clear();
            TriangleGenerator triangle;
            triangle.fillWavetable(wavetable);
            break;
        }
            
        case 3:
        {
            wavetable.clear();
            SquareGenerator square;
            square.fillWavetable(wavetable);
            break;
        }

        default:
            break;
            
    }
}

//==============================================================================
bool CynthiaAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *CynthiaAudioProcessor::createEditor()
{
    // return new CynthiaAudioProcessorEditor(*this);

    auto genericEditor = new juce::GenericAudioProcessorEditor(*this);
    genericEditor->setSize(500, 200);
    return genericEditor;
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
