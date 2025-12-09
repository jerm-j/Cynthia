#include "Cynthia_UI/FilterComponent.h"

FilterComponent::FilterComponent(APVTS &apvts) :    cutoffFrequencyAttachment(apvts, ParameterID::filterCutoff.getParamID(), cutoffFrequencyKnob),
                                                    resonanceLevelAttachment(apvts, ParameterID::filterResonance.getParamID(), resonanceLevelKnob),
                                                    filterTypeAttachment(apvts, ParameterID::filterType.getParamID(), filterTypeComboBox)
{
    configureKnob(cutoffFrequencyKnob);
    configureKnob(resonanceLevelKnob);

    configureComboBox(filterTypeComboBox, juce::StringArray{"LP", "HP", "BP"});
    
    configureComponentLabel(cutoffFrequencyLabel, juce::String("Cutoff Freq"));
    configureComponentLabel(resonanceLevelLabel, juce::String("Q"));
    configureComponentLabel(filterTypeLabel, juce::String("Filter Type"));
}

void FilterComponent::paint(juce::Graphics &g)
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

void FilterComponent::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    auto filterModuleArea = bounds;
    int knobSize = std::min(filterModuleArea.getWidth()/numComponents, filterModuleArea.getHeight()-30);
    int comboBoxSize = knobSize; //changed
    
    juce::FlexBox row;
    row.flexDirection = juce::FlexBox::Direction::row;
    row.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    row.alignItems = juce::FlexBox::AlignItems::center;

    auto cutoffFreqColumn = makeComponentWithLabel(cutoffFrequencyKnob, cutoffFrequencyLabel, knobSize/4, knobSize, knobSize, knobSize);
    auto resoLvlColumn = makeComponentWithLabel(resonanceLevelKnob, resonanceLevelLabel, knobSize/4, knobSize, knobSize, knobSize);
    auto filterTypeColumn = makeComponentWithLabel(filterTypeComboBox, filterTypeLabel, comboBoxSize/3, comboBoxSize, comboBoxSize/3, comboBoxSize);

    row.items.add(juce::FlexItem(cutoffFreqColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    row.items.add(juce::FlexItem(resoLvlColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    row.items.add(juce::FlexItem(filterTypeColumn).withFlex(1.0f).withMargin({5, 5, 5, 5}));
    
    row.performLayout(filterModuleArea);

    auto columnWidth = filterModuleArea.getWidth()/numComponents;
    auto columnBounds = filterModuleArea.removeFromLeft(columnWidth);

    cutoffFreqColumn.performLayout(columnBounds.reduced(5));
    resoLvlColumn.performLayout(filterModuleArea.removeFromLeft(columnWidth).reduced(5));
    filterTypeColumn.performLayout(filterModuleArea.removeFromLeft(columnWidth).reduced(5));
}

void FilterComponent::configureKnob(juce::Slider &knob)
{
    knob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    knob.setTextBoxStyle(
        juce::Slider::TextEntryBoxPosition::TextBoxBelow,
        false,
        50,
        15);

    addAndMakeVisible(knob);
}

void FilterComponent::configureComboBox(juce::ComboBox &comboBox, const juce::StringArray &items)
{
    comboBox.addItemList(items, 1);
    addAndMakeVisible(comboBox);
}

void FilterComponent::configureComponentLabel(juce::Label &componentLabel, const juce::String &componentLabelText)
{
    componentLabel.setText(componentLabelText, juce::dontSendNotification);
    componentLabel.setJustificationType(juce::Justification::centred);
    componentLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(componentLabel);
}

