#include "ADSRComponent.h"

ADSRComponent::ADSRComponent(APVTS &apvts) : attackLevelAttachment(apvts, ParameterID::envAttack.getParamID(), attackLevelKnob),
                                                                          decayLevelAttachment(apvts, ParameterID::envDecay.getParamID(), decayLevelKnob),
                                                                          sustainLevelAttachment(apvts, ParameterID::envSustain.getParamID(), sustainLevelKnob),
                                                                          releaseLevelAttachment(apvts, ParameterID::envRelease.getParamID(), releaseLevelKnob)
{
    configureKnob(attackLevelKnob);
    configureKnob(decayLevelKnob);
    configureKnob(sustainLevelKnob);
    configureKnob(releaseLevelKnob);

    configureComponentLabel(attackLevelLabel, "Attack");
    configureComponentLabel(decayLevelLabel, "Decay");
    configureComponentLabel(sustainLevelLabel, "Sustain");
    configureComponentLabel(releaseLevelLabel, "Release");
}

void ADSRComponent::paint(juce::Graphics &g)
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

void ADSRComponent::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    auto adsrModuleArea = bounds;
    int knobSize = std::min(adsrModuleArea.getWidth()/numComponents, adsrModuleArea.getHeight()-30);

    juce::FlexBox row;
    row.flexDirection = juce::FlexBox::Direction::row;
    row.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    row.alignItems = juce::FlexBox::AlignItems::center;

    auto attackColumn = makeComponentWithLabel(attackLevelKnob, attackLevelLabel, knobSize/4, knobSize, knobSize, knobSize);
    auto decayColumn = makeComponentWithLabel(decayLevelKnob, decayLevelLabel, knobSize/4, knobSize, knobSize, knobSize);
    auto sustainColumn = makeComponentWithLabel(sustainLevelKnob, sustainLevelLabel, knobSize/4, knobSize, knobSize, knobSize);
    auto releaseColumn = makeComponentWithLabel(releaseLevelKnob, releaseLevelLabel, knobSize/4, knobSize, knobSize, knobSize);
    
    row.items.add(juce::FlexItem(attackColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    row.items.add(juce::FlexItem(decayColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    row.items.add(juce::FlexItem(sustainColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    row.items.add(juce::FlexItem(releaseColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    
    row.performLayout(adsrModuleArea);

    auto columnWidth = adsrModuleArea.getWidth()/numComponents;
    auto columnBounds = adsrModuleArea.removeFromLeft(columnWidth);

    attackColumn.performLayout(columnBounds.reduced(5));
    decayColumn.performLayout(adsrModuleArea.removeFromLeft(columnWidth).reduced(5));
    sustainColumn.performLayout(adsrModuleArea.removeFromLeft(columnWidth).reduced(5));
    releaseColumn.performLayout(adsrModuleArea.removeFromLeft(columnWidth).reduced(5));
}

void ADSRComponent::configureKnob(juce::Slider &knob)
{
    knob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    knob.setTextBoxStyle(
        juce::Slider::TextEntryBoxPosition::TextBoxBelow,
        false,
        50,
        15);

    addAndMakeVisible(knob);
}

void ADSRComponent::configureComboBox(juce::ComboBox &comboBox, const juce::StringArray &items)
{
    comboBox.addItemList(items, 1);
    addAndMakeVisible(comboBox);
}

void ADSRComponent::configureComponentLabel(juce::Label &componentLabel, const juce::String &componentLabelText)
{
    componentLabel.setText(componentLabelText, juce::dontSendNotification);
    componentLabel.setJustificationType(juce::Justification::centred);
    componentLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(componentLabel);
}