#pragma once

#include "Cynthia_UI/SynthUIModule.h"


class ADSRComponent : public SynthUIModule
{
public:

    ADSRComponent(APVTS& apvts);
    
private:

    void paint(juce::Graphics &g) override;
    void resized() override;

    void configureKnob(juce::Slider &knob) override;
    void configureComboBox(juce::ComboBox &comboBox, const juce::StringArray &items) override;

    void configureComponentLabel(juce::Label &componentLabel, const juce::String &componentLabelText) override;

    const juce::String moduleHeader = "ADSR";
    const int numComponents = 4;

    juce::Slider attackLevelKnob;
    juce::Slider decayLevelKnob;
    juce::Slider sustainLevelKnob;
    juce::Slider releaseLevelKnob;

    juce::Label attackLevelLabel;
    juce::Label decayLevelLabel;
    juce::Label sustainLevelLabel;
    juce::Label releaseLevelLabel;

    // these lines must go below the knob declarations
    // when the editor window closes, member objects are destroyed in reverse order 
    // of declaration, and the attachment must be destroyed before the slider it's attached to.
    SliderAttachment attackLevelAttachment;
    SliderAttachment decayLevelAttachment;
    SliderAttachment sustainLevelAttachment;
    SliderAttachment releaseLevelAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRComponent)
};