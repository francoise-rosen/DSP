/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveshaperAudioProcessorEditor::WaveshaperAudioProcessorEditor (WaveshaperAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
    // upon startup read the value off the processor?
{

    setSize (400, 250);
    addAndMakeVisible(&gainSlider);
    gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 40);

    addAndMakeVisible(&dBLabel);
    dBLabel.setText("GAIN", dontSendNotification);
    dBLabel.attachToComponent(&gainSlider, false);
    
    addAndMakeVisible(&saturationSlider);
    saturationSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    saturationSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 30);
    
    addAndMakeVisible(&symmetrySlider);
    symmetrySlider.setSliderStyle(Slider::SliderStyle::Rotary);
    symmetrySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 30);
    
    addAndMakeVisible(&crossfadeSlider);
    crossfadeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    crossfadeSlider.setTextBoxStyle(Slider::TextBoxAbove, false, 70, 30);
    
    // tree state attachments
    gainSliderAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.accessTreeState(), WaveshaperAudioProcessor::paramGain, gainSlider));
    
    saturationSliderAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.accessTreeState(),
        WaveshaperAudioProcessor::paramSaturation, saturationSlider));
    
    symmetrySliderAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.accessTreeState(), WaveshaperAudioProcessor::paramSymmetry, symmetrySlider));
    
    crossfadeSliderAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.accessTreeState(), WaveshaperAudioProcessor::paramCrossfade, crossfadeSlider));
    
    functions_A_ComboAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(processor.accessTreeState(), WaveshaperAudioProcessor::paramTransferFunctionListA, functions_A_Combo));
    
    functions_B_ComboAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(processor.accessTreeState(), WaveshaperAudioProcessor::paramTransferFunctionListB, functions_B_Combo));
    

    
}

WaveshaperAudioProcessorEditor::~WaveshaperAudioProcessorEditor()
{
}

//==============================================================================
void WaveshaperAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

void WaveshaperAudioProcessorEditor::resized()
{
    
    auto box = getLocalBounds().reduced(20);
    gainSlider.setBounds(box.removeFromRight(70).removeFromBottom(120));
    saturationSlider.setBounds(box.removeFromLeft(70).removeFromBottom(120));
    symmetrySlider.setBounds(box.removeFromBottom(220));
    crossfadeSlider.setBounds(box.removeFromLeft(70).removeFromTop(120));
}

