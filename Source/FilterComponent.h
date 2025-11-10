#pragma once

#include "SynthUIModule.h"


class FilterComponent : public SynthUIModule
{
public:

    FilterComponent(APVTS &apvts);

private:

    void paint(juce::Graphics&) override;
    void resized() override;

    void configureKnob(juce::Slider &knob) override;

    void configureComboBox(juce::ComboBox &comboBox, const juce::StringArray &items) override;

    void configureComponentLabel(juce::Label &componentLabel, const juce::String &componentLabelText) override;

    const juce::String moduleHeader = "Filter";
    const int numComponents = 3;

    juce::Slider cutoffFrequencyKnob;
    juce::Slider resonanceLevelKnob;
    juce::ComboBox filterTypeComboBox;

    juce::Label cutoffFrequencyLabel;
    juce::Label resonanceLevelLabel;
    juce::Label filterTypeLabel;

    // these lines must go below the knob declarations
    // when the editor window closes, member objects are destroyed in reverse order 
    // of declaration, and the attachment must be destroyed before the slider it's attached to.
    SliderAttachment cutoffFrequencyAttachment;
    SliderAttachment resonanceLevelAttachment;
    ComboBoxAttachment filterTypeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterComponent)
};