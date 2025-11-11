#pragma once
#include "SynthUIModule.h"

class OscillatorComponent : public SynthUIModule
{
public:
    OscillatorComponent(APVTS &apvts);

private:
    void paint(juce::Graphics &g);
    void resized();

    void configureKnob(juce::Slider &knob) override;
    void configureComboBox(juce::ComboBox &comboBox, const juce::StringArray &items) override;
    void configureComponentLabel(juce::Label &componentLabel, const juce::String &componentLabelText) override;

    const juce::String moduleHeader = "Oscillator";
    const int numComponents = 4;

    juce::Slider morphValueKnob;
    juce::Slider detuneCentsKnob;
    juce::ComboBox wavetypeAComboBox;
    juce::ComboBox wavetypeBComboBox;

    juce::Label morphValueLabel;
    juce::Label detuneCentsLabel;
    juce::Label wavetypeALabel;
    juce::Label wavetypeBLabel;

    SliderAttachment morphValueKnobAttachment;
    SliderAttachment detuneDentsKnobAttachment;
    ComboBoxAttachment wavetypeAComboBoxAttachment;
    ComboBoxAttachment wavetypeBComboBoxAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorComponent)
};