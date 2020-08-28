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
    freqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    addAndMakeVisible(&freqSlider);

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    addAndMakeVisible(&gainSlider);
    
    freqSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.freqParam, freqSlider);
    gainSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.getValueTree(), audioProcessor.gainParam, gainSlider);
    
    border = std::make_unique<int>(10);
    dialWidth = 100;
    
    setResizable(true, true);
    setResizeLimits(200, 100, 500, 350);
    setSize (300, 200);
}

OnePoleAudioProcessorEditor::~OnePoleAudioProcessorEditor()
{
}

//==============================================================================
void OnePoleAudioProcessorEditor::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::honeydew);
    g.setFont(14.0f);
    
    auto lineThickness = 3.0f;
    
    auto guiArea = getLocalBounds().reduced(*border);
    dialWidth = guiArea.getWidth() / 2 - (*border)/2;
    g.setColour(juce::Colours::cyan);
    g.drawRoundedRectangle(guiArea.toFloat(), 10, lineThickness);

    g.setColour(juce::Colours::darkcyan);
    
    
   g.drawRoundedRectangle(guiArea.withRight((getWidth()/2) - (*border)/2).withLeft(guiArea.getX()).reduced(lineThickness).toFloat(), 10.0f, lineThickness);
       g.drawRoundedRectangle(guiArea.withLeft(getWidth()/2 + (*border)/2).reduced(lineThickness).toFloat(), 10.0f, lineThickness);
    
    g.setColour(juce::Colours::honeydew);
    
    g.drawFittedText("FREQUENCY", guiArea.removeFromLeft(dialWidth).reduced(*border), juce::Justification::centredTop, 1);
    g.drawFittedText("GAIN", guiArea.withLeft(dialWidth - *border).reduced(*border), juce::Justification::centredTop, 1);
    

}

void OnePoleAudioProcessorEditor::resized()
{
    auto X = getWidth();
    auto Y = getHeight();
    *border = juce::jmin(10, juce::jmin(X, Y) / 20);
  
    auto dialArea = getLocalBounds().removeFromBottom(getHeight()- (*border)).reduced(*border);
    
    dialWidth = dialArea.getWidth() / 2;
    
    freqSlider.setBounds(dialArea.removeFromLeft(dialWidth).reduced(*border));
    gainSlider.setBounds(dialArea.removeFromLeft(dialWidth).reduced(*border));
    
}
