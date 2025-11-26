#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CynthiaAudioProcessorEditor::CynthiaAudioProcessorEditor (CynthiaAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), adsrUI(p.apvts), filterUI(p.apvts), oscillatorUI(p.apvts), lfoUI(p.apvts)
{
    addAndMakeVisible(adsrUI);
    addAndMakeVisible(filterUI);
    addAndMakeVisible(oscillatorUI);
    addAndMakeVisible(lfoUI);
    setSize(1000, 500);
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
    auto editorBounds = getLocalBounds();

    int width = editorBounds.getWidth()/2;
    int height = editorBounds.getHeight()/2;

    adsrUI.setBounds(0, 0, width, height);
    filterUI.setBounds(adsrUI.getWidth(), 0, width, height);
    oscillatorUI.setBounds(0, adsrUI.getHeight(), width, height);
    lfoUI.setBounds(oscillatorUI.getWidth(), filterUI.getHeight(), width, height);
}
