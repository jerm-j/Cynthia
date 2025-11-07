#include "ADSRComponent.h"

ADSRComponent::ADSRComponent(juce::AudioProcessorValueTreeState &apvts) : 
    attackLevelAttachment(apvts, ParameterID::envAttack.getParamID(), attackLevelKnob),
    decayLevelAttachment(apvts, ParameterID::envDecay.getParamID(), decayLevelKnob),
    sustainLevelAttachment(apvts, ParameterID::envSustain.getParamID(), sustainLevelKnob),
    releaseLevelAttachment(apvts, ParameterID::envRelease.getParamID(), releaseLevelKnob)
{
    attackLevelKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    decayLevelKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sustainLevelKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    releaseLevelKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);

    addAndMakeVisible(attackLevelKnob);
    addAndMakeVisible(decayLevelKnob);
    addAndMakeVisible(sustainLevelKnob);
    addAndMakeVisible(releaseLevelKnob);
}

ADSRComponent::~ADSRComponent()
{
    
}

void ADSRComponent::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::red);
    g.drawRect(getLocalBounds(), 1);
    g.drawText(
        "ADSR", 
        getLocalBounds()
            .removeFromTop(20),
        juce::Justification::centred
    );
}

void ADSRComponent::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    auto knobArea = bounds.removeFromTop(bounds.getHeight()-20);

    auto width = knobArea.getWidth()/4;
    attackLevelKnob.setBounds(knobArea.removeFromLeft(width));
    decayLevelKnob.setBounds(knobArea.removeFromLeft(width));
    sustainLevelKnob.setBounds(knobArea.removeFromLeft(width));
    releaseLevelKnob.setBounds(knobArea);
}
