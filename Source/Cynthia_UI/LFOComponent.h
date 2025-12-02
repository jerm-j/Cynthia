#pragma once
#include "Cynthia_UI/SynthUIModule.h"

class LFOComponent : public SynthUIModule
{
public:
    LFOComponent(APVTS &apvts);

private:
    void paint(juce::Graphics &g);
    void resized();

    void configureKnob(juce::Slider &knob) override;
    void configureComboBox(juce::ComboBox &comboBox, const juce::StringArray &items) override;
    void configureComponentLabel(juce::Label &componentLabel, const juce::String &componentLabelText) override;

    const juce::String moduleHeader = "LFO";
    const int numComponents = 6;

    juce::Slider morphValueKnob;
    juce::Slider detuneCentsKnob;
    juce::Slider modDepthKnob;
    juce::Slider modFreqKnob;
    juce::ComboBox wavetypeAComboBox;
    juce::ComboBox wavetypeBComboBox;

    juce::Label morphValueLabel;
    juce::Label detuneCentsLabel;
    juce::Label modDepthLabel;
    juce::Label modFreqLabel;
    juce::Label wavetypeALabel;
    juce::Label wavetypeBLabel;

    SliderAttachment morphValueKnobAttachment;
    SliderAttachment detuneDentsKnobAttachment;
    SliderAttachment modDepthKnobAttachment;
    SliderAttachment modFreqKnobAttachment;
    ComboBoxAttachment wavetypeAComboBoxAttachment;
    ComboBoxAttachment wavetypeBComboBoxAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};