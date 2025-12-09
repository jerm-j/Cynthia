/*

    PluginEditor.cpp

    Our main editor where we hold reference to all UI modules
    and make them visible to the editor


*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CynthiaAudioProcessorEditor::CynthiaAudioProcessorEditor (CynthiaAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), adsrUI(p.apvts), filterUI(p.apvts), oscillatorUI(p.apvts), lfoUI(p.apvts)
{
    addAndMakeVisible(oscillatorUI);
    addAndMakeVisible(filterUI);
    addAndMakeVisible(adsrUI);
    addAndMakeVisible(lfoUI);
    setSize(900, 400);
}

CynthiaAudioProcessorEditor::~CynthiaAudioProcessorEditor()
{
}

//==============================================================================
void CynthiaAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void CynthiaAudioProcessorEditor::resized()
{
    auto editorBounds = getLocalBounds();

    int width = editorBounds.getWidth()/2;
    int height = editorBounds.getHeight()/2;

    oscillatorUI.setBounds(0,0, width, height);
    filterUI.setBounds(oscillatorUI.getWidth(), 0, width, height);
    adsrUI.setBounds(0, oscillatorUI.getHeight(), width, height);
    lfoUI.setBounds(adsrUI.getWidth(), filterUI.getHeight(), width, height);
}
