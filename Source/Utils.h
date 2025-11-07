/*
    Utils.h

    A place to store any utility functions to do quick math or DSP.

    protectYourEars() source: "Creating Synthesizer Plugins Using C++ and JUCE" by Matthijs Hollemans
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
        PARAMETER_ID(wavetype)
        PARAMETER_ID(polyMode)
        PARAMETER_ID(envAttack)
        PARAMETER_ID(envDecay)
        PARAMETER_ID(envSustain)
        PARAMETER_ID(envRelease)
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
