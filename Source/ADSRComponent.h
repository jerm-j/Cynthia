#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Utils.h"


class ADSRComponent : public juce::Component
{
public:

    ADSRComponent(juce::AudioProcessorValueTreeState& apvts);
    ~ADSRComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider attackLevelKnob;
    juce::Slider decayLevelKnob;
    juce::Slider sustainLevelKnob;
    juce::Slider releaseLevelKnob;

    // these lines must go below the knob declarations
    // when the editor window closes, member objects are destroyed in reverse order 
    // of declaration, and the attachment must be destroyed before the slider it's attached to.
    using APVTS = juce::AudioProcessorValueTreeState;
    using SliderAttachment = APVTS::SliderAttachment;
    
    SliderAttachment attackLevelAttachment;
    SliderAttachment decayLevelAttachment;
    SliderAttachment sustainLevelAttachment;
    SliderAttachment releaseLevelAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRComponent)
};