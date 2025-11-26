#pragma once

#include "PluginProcessor.h"
#include "ADSRComponent.h"
#include "FilterComponent.h"
#include "OscillatorComponent.h"
#include "LFOComponent.h"

//==============================================================================
class CynthiaAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit CynthiaAudioProcessorEditor (CynthiaAudioProcessor&);
    ~CynthiaAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CynthiaAudioProcessor& processorRef;
    ADSRComponent adsrUI;
    FilterComponent filterUI;
    OscillatorComponent oscillatorUI;
    LFOComponent lfoUI;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CynthiaAudioProcessorEditor)
};
