/*
    Utils.h

    A place to store any utility functions and namespaces.

    source: "Creating Synthesizer Plugins Using C++ and JUCE" by Matthijs Hollemans
*/


#pragma once

#include <cmath>
#include <juce_audio_processors/juce_audio_processors.h>
#include <cstring>

/*
    Here we create a new namespace which contains juce::ParameterID objects for each parameter definition.
    To get the identifier of a parameter, we can just write "ParameterID::paramID".
    The actual string value is just "paramID".
*/
namespace ParameterID
{
    #define PARAMETER_ID(str) const juce::ParameterID str(#str, 1);
        PARAMETER_ID(wavetypeAOsc)
        PARAMETER_ID(wavetypeBOsc)
        PARAMETER_ID(morphValueOsc)
        PARAMETER_ID(detuneCentsOsc)
        PARAMETER_ID(wavetypeALFO)
        PARAMETER_ID(wavetypeBLFO)
        PARAMETER_ID(morphValueLFO)
        PARAMETER_ID(detuneCentsLFO)
        PARAMETER_ID(modDepthLFO)
        PARAMETER_ID(modFreqLFO)
        PARAMETER_ID(polyMode)
        PARAMETER_ID(envAttack)
        PARAMETER_ID(envDecay)
        PARAMETER_ID(envSustain)
        PARAMETER_ID(envRelease)
        PARAMETER_ID(filterType)
        PARAMETER_ID(filterCutoff)
        PARAMETER_ID(filterResonance)
        PARAMETER_ID(outputGain)
    #undef PARAMETER_ID
}

template<typename T>
inline static void castParameter(juce::AudioProcessorValueTreeState& apvts,
                                const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination); // ensure parameter exists and is not wrong type
};

inline juce::AudioProcessorValueTreeState::ParameterLayout configureLayout() 
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    // the ParameterLayout assumes ownership of the AudioParameter object, which is why
    // the parameter is constructed using std::make_unique

    /*
        Oscillator Params
    */
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        ParameterID::wavetypeAOsc,                                       // the identifier
        "Wavetype A",                                                  // human readable name of the parameter (this is what the DAW shows to the user)
        juce::StringArray{"Sine", "Sawtooth", "Triangle", "Square"}, // the list of wavetypes to choose from
        0));                                                         // the default choice (Sine)
        
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        ParameterID::wavetypeBOsc,                                       // the identifier
        "Wavetype B",                                                  // human readable name of the parameter (this is what the DAW shows to the user)
        juce::StringArray{"Sine", "Sawtooth", "Triangle", "Square"}, // the list of wavetypes to choose from
        1));                                                         // the default choice (Saw)

    layout.add(std::make_unique<juce::AudioParameterFloat>(
       ParameterID::morphValueOsc,
       "Morph",
       juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
       0.0f // default: no morphing 
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::detuneCentsOsc,
        "Detune",
        juce::NormalisableRange<float>(-100.0f, 100.0f, 0.01f),
        0.0f
    ));

    /*
        LFO Params
    */
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        ParameterID::wavetypeALFO,                                       // the identifier
        "Wavetype A",                                                  // human readable name of the parameter (this is what the DAW shows to the user)
        juce::StringArray{"Sine", "Sawtooth", "Triangle", "Square"}, // the list of wavetypes to choose from
        0));                                                         // the default choice (Sine)
        
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        ParameterID::wavetypeBLFO,                                       // the identifier
        "Wavetype B",                                                  // human readable name of the parameter (this is what the DAW shows to the user)
        juce::StringArray{"Sine", "Sawtooth", "Triangle", "Square"}, // the list of wavetypes to choose from
        1));                                                         // the default choice (Saw)

    layout.add(std::make_unique<juce::AudioParameterFloat>(
       ParameterID::morphValueLFO,
       "Morph",
       juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
       0.0f // default: no morphing 
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::detuneCentsLFO,
        "Detune",
        juce::NormalisableRange<float>(-100.0f, 100.0f, 0.01f),
        0.0f
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::modDepthLFO,
        "Mod Depth",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01),
        0.0f
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::modFreqLFO,
        "Mod Freq",
        juce::NormalisableRange<float>(0.0f, 500.0f, 0.01),
        0.0f
    ));

    /*
        Polyphony Param
    */

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        ParameterID::polyMode,                         
        "Polyphony Mode",                              
        juce::StringArray{"Monophonic", "Polyphonic"}, 
        1));                                           

    /*

        ADSR Params


        Note on Envelope ADSR params:
            Attack, Decay, Sustain, and Release are measured in seconds
            

            "Sustain is a percentage of the amplitude level... it is not a time
            but a level. It determines how load the sound is during the steady part...
            Sustain level does not change based on the note velocity... no matter how loud
            or how quiet it is, the sustain level is always relative to the note's amplitude."
    */
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::envAttack,
        "Env Attack",
        juce::NormalisableRange<float>(0.0f, 5.0f, 0.001f),
        0.01f,
        juce::AudioParameterFloatAttributes().withLabel("s")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::envDecay,
        "Env Decay",
        juce::NormalisableRange<float>(0.0f, 5.0f, 0.001f),
        0.1f,
        juce::AudioParameterFloatAttributes().withLabel("s")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::envSustain,
        "Env Sustain",
        juce::NormalisableRange<float>(0.0f, 0.8f, 0.001f), // was previously maxed at 1
        1.0f,
        juce::AudioParameterFloatAttributes().withLabel("")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::envRelease,
        "Env Release",
        juce::NormalisableRange<float>(0.0f, 5.0f, 0.001f),
        0.5f,
        juce::AudioParameterFloatAttributes().withLabel("s")));

    /*
        Filter Params
    */
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        ParameterID::filterType,
        "Filter Type",
        juce::StringArray{"LowPass", "HighPass", "BandPass"},
        0));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::filterCutoff,
        "Filter Cutoff",
        juce::NormalisableRange<float>(1.0f, 20000.0f, 1.0f),
        10000.0f,
        juce::AudioParameterFloatAttributes().withLabel("Hz")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::filterResonance,
        "Filter Resonance",
        juce::NormalisableRange<float>(0.01f, 1.0f, 0.01f),
        0.5f,
        juce::AudioParameterFloatAttributes().withLabel("Q")));

    /*
        Final Output Gain Param
    */
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::outputGain,
        "Output Gain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.1),
        0.3f,
        juce::AudioParameterFloatAttributes()));

    return layout;
};
