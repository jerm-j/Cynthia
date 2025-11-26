#include "OscillatorComponent.h"

OscillatorComponent::OscillatorComponent(APVTS &apvts) : morphValueKnobAttachment(apvts, ParameterID::morphValueOsc.getParamID(), morphValueKnob),
                                                         detuneDentsKnobAttachment(apvts, ParameterID::detuneCentsOsc.getParamID(), detuneCentsKnob),
                                                         wavetypeAComboBoxAttachment(apvts, ParameterID::wavetypeAOsc.getParamID(), wavetypeAComboBox),
                                                         wavetypeBComboBoxAttachment(apvts, ParameterID::wavetypeBOsc.getParamID(), wavetypeBComboBox)
                                                         
{
    configureKnob(morphValueKnob);
    configureKnob(detuneCentsKnob);
    configureComboBox(wavetypeAComboBox, juce::StringArray{"Sine", "Saw", "Triangle", "Square"});
    configureComboBox(wavetypeBComboBox, juce::StringArray{"Sine", "Saw", "Triangle", "Square"});

    configureComponentLabel(morphValueLabel, juce::String("Morph"));
    configureComponentLabel(detuneCentsLabel, "Detune");
    configureComponentLabel(wavetypeALabel, juce::String("Wavetype A"));
    configureComponentLabel(wavetypeBLabel, juce::String("Wavetype B"));
}

void OscillatorComponent::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::red);
    g.drawRect(getLocalBounds(), 1);
    g.drawText(
        moduleHeader,
        getLocalBounds()
            .removeFromTop(20),
        juce::Justification::centred);
}

void OscillatorComponent::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    auto oscModuleArea = bounds;
    int knobSize = std::min(oscModuleArea.getWidth()/numComponents, oscModuleArea.getHeight()-30);
    int comboBoxSize = knobSize; // changed
    
    juce::FlexBox row;
    row.flexDirection = juce::FlexBox::Direction::row;
    row.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    row.alignItems = juce::FlexBox::AlignItems::center;

    auto morphColumn = makeComponentWithLabel(morphValueKnob, morphValueLabel, knobSize/4, knobSize, knobSize, knobSize);
    auto detuneColumn = makeComponentWithLabel(detuneCentsKnob, detuneCentsLabel, knobSize/4, knobSize, knobSize, knobSize);
    auto wavetypeAColumn = makeComponentWithLabel(wavetypeAComboBox, wavetypeALabel, comboBoxSize/3, comboBoxSize, comboBoxSize/3, comboBoxSize);
    auto wavetypeBColumn = makeComponentWithLabel(wavetypeBComboBox, wavetypeBLabel, comboBoxSize/3, comboBoxSize, comboBoxSize/3, comboBoxSize);

    row.items.add(juce::FlexItem(morphColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    row.items.add(juce::FlexItem(detuneColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    row.items.add(juce::FlexItem(wavetypeAColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    row.items.add(juce::FlexItem(wavetypeBColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));

    row.performLayout(oscModuleArea);

    auto columnWidth = oscModuleArea.getWidth()/numComponents;
    auto columnBounds = oscModuleArea.removeFromLeft(columnWidth);

    morphColumn.performLayout(columnBounds.reduced(5));
    detuneColumn.performLayout(oscModuleArea.removeFromLeft(columnWidth).reduced(5));
    wavetypeAColumn.performLayout(oscModuleArea.removeFromLeft(columnWidth).reduced(5));
    wavetypeBColumn.performLayout(oscModuleArea.removeFromLeft(columnWidth).reduced(5));
}

void OscillatorComponent::configureKnob(juce::Slider &knob)
{
    knob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    knob.setTextBoxStyle(
        juce::Slider::TextEntryBoxPosition::TextBoxBelow,
        false,
        50,
        15);

    addAndMakeVisible(knob);
}

void OscillatorComponent::configureComboBox(juce::ComboBox &comboBox, const juce::StringArray &items)
{
    comboBox.addItemList(items, 1);
    addAndMakeVisible(comboBox);
}

void OscillatorComponent::configureComponentLabel(juce::Label &componentLabel, const juce::String &componentLabelText)
{
    componentLabel.setText(componentLabelText, juce::dontSendNotification);
    componentLabel.setJustificationType(juce::Justification::centred);
    componentLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(componentLabel);
}
