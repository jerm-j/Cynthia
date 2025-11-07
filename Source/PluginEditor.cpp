#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CynthiaAudioProcessorEditor::CynthiaAudioProcessorEditor (CynthiaAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), adsrUI(p.apvts)
{
    addAndMakeVisible(adsrUI);
    setSize(400, 200);
}

CynthiaAudioProcessorEditor::~CynthiaAudioProcessorEditor()
{
}

//==============================================================================
void CynthiaAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // g.setColour (juce::Colours::white);
    // g.setFont (15.0f);
}

void CynthiaAudioProcessorEditor::resized()
{
    adsrUI.setBounds(getLocalBounds().reduced(50));
}
