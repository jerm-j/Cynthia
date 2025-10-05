#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CynthiaAudioProcessor::CynthiaAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
}

CynthiaAudioProcessor::~CynthiaAudioProcessor()
{
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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CynthiaAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CynthiaAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String CynthiaAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void CynthiaAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void CynthiaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void CynthiaAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

// Called by the DAW to query the number of channels supported
bool CynthiaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void CynthiaAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    /* 
        Denormals are floating point numbers that are so small, they are practically 0.
        Denormals are slower than regular floating point numbers, so we treat them as 0.
     
        juce::ScopedNoDenormals sets a CPU flag for this method to truncate floating point numbers
        to 0 instead of turning them into denormal numbers.
    */
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // JUCE does not guarantee the AudioBuffer is already cleared, so we must clear it of potential garbage values.
    // this is to prevent, in the worst case, screaming feedback!
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    splitBufferByEvents(buffer, midiMessages);
}

// Source: "Creating Synthesizer Plug-ins with C++ and JUCE" by Matthijs Hollemans
void CynthiaAudioProcessor::splitBufferByEvents(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    int bufferOffset = 0;
  
    for(const auto metadata : midiMessages)
    {
        // Render the audio that happens before this event (if any)
        int samplesThisSegment = metadata.samplePosition - bufferOffset;
        if(samplesThisSegment > 0)
        {
            render(buffer, samplesThisSegment, bufferOffset);
            bufferOffset += samplesThisSegment;
        }

        // Handle the event. Ignore midi messages such as sysex.
        if(metadata.numBytes <= 3) 
        {
            uint8_t data1 = (metadata.numBytes >= 2) ? metadata.data[1] : 0;
            uint8_t data2 = (metadata.numBytes == 3) ? metadata.data[2] : 0;
            handleMIDI(metadata.data[0], data1, data2);
        }

        /* 
            Render the audio after the last MIDI event. If there were no
            MIDI events at all, this renders the entire buffer.
        */
        int samplesLastSegment = buffer.getNumSamples() - bufferOffset;
        if(samplesLastSegment > 0)
        {
            render(buffer, samplesLastSegment, bufferOffset);
        }

        midiMessages.clear();
    }
}

// Source: "Creating Synthesizer Plug-ins with C++ and JUCE" by Matthijs Hollemans
void CynthiaAudioProcessor::handleMIDI(uint8_t data0, uint8_t data1, uint8_t data2) 
{
    char s[16];
    snprintf(s, 16, "%02hhX %02hhX %02hhX", data0, data1, data2);
    DBG(s);
}

// Source: "Creating Synthesizer Plug-ins with C++ and JUCE" by Matthijs Hollemans
void CynthiaAudioProcessor::render(juce::AudioBuffer<float>& buffer, int sampleCount, int bufferOffest)
{
    // need to implement once there is audio to output
}

//==============================================================================
bool CynthiaAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CynthiaAudioProcessor::createEditor()
{
    return new CynthiaAudioProcessorEditor (*this);
}

//==============================================================================
void CynthiaAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void CynthiaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CynthiaAudioProcessor();
}
