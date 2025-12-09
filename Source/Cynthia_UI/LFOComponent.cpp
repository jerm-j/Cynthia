#include "Cynthia_UI/LFOComponent.h"

LFOComponent::LFOComponent(APVTS &apvts) :  morphValueKnobAttachment(apvts, ParameterID::morphValueLFO.getParamID(), morphValueKnob),
                                            detuneDentsKnobAttachment(apvts, ParameterID::detuneCentsLFO.getParamID(), detuneCentsKnob),
                                            modDepthKnobAttachment(apvts, ParameterID::modDepthLFO.getParamID(), modDepthKnob),
                                            modFreqKnobAttachment(apvts, ParameterID::modFreqLFO.getParamID(), modFreqKnob),
                                            wavetypeAComboBoxAttachment(apvts, ParameterID::wavetypeALFO.getParamID(), wavetypeAComboBox),
                                            wavetypeBComboBoxAttachment(apvts, ParameterID::wavetypeBLFO.getParamID(), wavetypeBComboBox)
{
    configureKnob(morphValueKnob);
    configureKnob(detuneCentsKnob);
    configureKnob(modDepthKnob);
    configureKnob(modFreqKnob);
    configureComboBox(wavetypeAComboBox, juce::StringArray{"Sine", "Saw", "Triangle", "Square"});
    configureComboBox(wavetypeBComboBox, juce::StringArray{"Sine", "Saw", "Triangle", "Square"});

    configureComponentLabel(morphValueLabel, juce::String("Morph"));
    configureComponentLabel(detuneCentsLabel, juce::String("Detune"));
    configureComponentLabel(modDepthLabel, juce::String("Mod Depth"));
    configureComponentLabel(modFreqLabel, juce::String("Mod Freq"));
    configureComponentLabel(wavetypeALabel, juce::String("Wavetype A"));
    configureComponentLabel(wavetypeBLabel, juce::String("Wavetype B"));
}

void LFOComponent::paint(juce::Graphics &g)
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

void LFOComponent::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    auto lfoModuleArea = bounds;
    int knobSize = std::min(lfoModuleArea.getWidth()/numComponents, lfoModuleArea.getHeight()-30);
    int comboBoxSize = knobSize; // changed
    
    juce::FlexBox row;
    row.flexDirection = juce::FlexBox::Direction::row;
    row.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    row.alignItems = juce::FlexBox::AlignItems::center;

    auto morphColumn = makeComponentWithLabel(morphValueKnob, morphValueLabel, knobSize/4, knobSize, knobSize, knobSize);
    auto detuneColumn = makeComponentWithLabel(detuneCentsKnob, detuneCentsLabel, knobSize/4, knobSize, knobSize, knobSize);
    auto modDepthColumn = makeComponentWithLabel(modDepthKnob, modDepthLabel, knobSize/4, knobSize, knobSize, knobSize);
    auto modFreqColumn = makeComponentWithLabel(modFreqKnob, modFreqLabel, knobSize/4, knobSize, knobSize, knobSize);
    auto wavetypeAColumn = makeComponentWithLabel(wavetypeAComboBox, wavetypeALabel, comboBoxSize/3, comboBoxSize, comboBoxSize/3, comboBoxSize);
    auto wavetypeBColumn = makeComponentWithLabel(wavetypeBComboBox, wavetypeBLabel, comboBoxSize/3, comboBoxSize, comboBoxSize/3, comboBoxSize);

    row.items.add(juce::FlexItem(morphColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    row.items.add(juce::FlexItem(detuneColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    row.items.add(juce::FlexItem(modDepthColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    row.items.add(juce::FlexItem(modFreqColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    row.items.add(juce::FlexItem(wavetypeAColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    row.items.add(juce::FlexItem(wavetypeBColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));

    row.performLayout(lfoModuleArea);

    auto columnWidth = lfoModuleArea.getWidth()/numComponents;
    auto columnBounds = lfoModuleArea.removeFromLeft(columnWidth);

    morphColumn.performLayout(columnBounds.reduced(5));
    detuneColumn.performLayout(lfoModuleArea.removeFromLeft(columnWidth).reduced(5));
    modDepthColumn.performLayout(lfoModuleArea.removeFromLeft(columnWidth).reduced(5));
    modFreqColumn.performLayout(lfoModuleArea.removeFromLeft(columnWidth).reduced(5));
    wavetypeAColumn.performLayout(lfoModuleArea.removeFromLeft(columnWidth).reduced(5));
    wavetypeBColumn.performLayout(lfoModuleArea.removeFromLeft(columnWidth).reduced(5));
}

void LFOComponent::configureKnob(juce::Slider &knob)
{
    knob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    knob.setTextBoxStyle(
        juce::Slider::TextEntryBoxPosition::TextBoxBelow,
        false,
        50,
        15);

    addAndMakeVisible(knob);
}

void LFOComponent::configureComboBox(juce::ComboBox &comboBox, const juce::StringArray &items)
{
    comboBox.addItemList(items, 1);
    addAndMakeVisible(comboBox);
}

void LFOComponent::configureComponentLabel(juce::Label &componentLabel, const juce::String &componentLabelText)
{
    componentLabel.setText(componentLabelText, juce::dontSendNotification);
    componentLabel.setJustificationType(juce::Justification::centred);
    componentLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(componentLabel);
}
