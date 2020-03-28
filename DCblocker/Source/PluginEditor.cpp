/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DcblockerAudioProcessorEditor::DcblockerAudioProcessorEditor (DcblockerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (200, 150);
    addAndMakeVisible(&dcslider);
    dcslider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    dcslider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    
    filterSliderAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.accessTreeState(), DCBID, dcslider));
}

DcblockerAudioProcessorEditor::~DcblockerAudioProcessorEditor()
{
}

//==============================================================================
void DcblockerAudioProcessorEditor::paint (Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

void DcblockerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    dcslider.setBounds(area.reduced(29));
}
