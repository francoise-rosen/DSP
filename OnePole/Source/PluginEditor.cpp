/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OnePoleAudioProcessorEditor::OnePoleAudioProcessorEditor (OnePoleAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    freqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    addAndMakeVisible(&freqSlider);

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    addAndMakeVisible(&gainSlider);
    
    freqSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.freqParam, freqSlider);
    gainSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.gainParam, gainSlider);
    
    setSize (400, 300);
}

OnePoleAudioProcessorEditor::~OnePoleAudioProcessorEditor()
{
}

//==============================================================================
void OnePoleAudioProcessorEditor::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::honeydew);
    g.setFont(24.0f);
    
    auto dialArea = getLocalBounds().reduced(edge);
    dialArea = dialArea.withTop(dialArea.getBottom() - 40);
    
    auto nameArea = getLocalBounds().removeFromLeft(getWidth()/4).removeFromTop(getHeight()/4).reduced(edge);
    
    g.drawFittedText("Rosen::OnePole LPF", nameArea, juce::Justification::centred, 1);
    
    const auto dialWidth = dialArea.getWidth() / 2;
    g.drawFittedText("FREQUENCY", dialArea.removeFromLeft(dialWidth), juce::Justification::centred, 1);
    g.drawFittedText("GAIN", dialArea.removeFromLeft(dialWidth), juce::Justification::centred, 1);
    
    g.setColour(juce::Colours::yellow);

}

void OnePoleAudioProcessorEditor::resized()
{
  
    auto dialArea = getLocalBounds().reduced(edge);
    const auto dialWidth = dialArea.getWidth() / 2;
    
    freqSlider.setBounds(dialArea.removeFromLeft(dialWidth).reduced(edge / 2));
    gainSlider.setBounds(dialArea.removeFromLeft(dialWidth).reduced(edge / 2));
    
}
