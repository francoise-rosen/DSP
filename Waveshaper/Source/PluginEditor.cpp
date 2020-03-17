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

{
    
    getLookAndFeel().setColour(Slider::trackColourId, Colours::darkorchid);
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::mistyrose);

    setSize (500, 250);
    gainSlider.setName("Gain");
    addAndMakeVisible(&gainSlider);
    gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 2);

    saturationSlider.setName("Saturation");
    addAndMakeVisible(&saturationSlider);
    saturationSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    saturationSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    
    symmetrySlider.setName("Symmetry");
    addAndMakeVisible(&symmetrySlider);
    symmetrySlider.setSliderStyle(Slider::SliderStyle::Rotary);
    symmetrySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    
    crossfadeSlider.setName("Crossfade");
    addAndMakeVisible(&crossfadeSlider);
    crossfadeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    crossfadeSlider.setTextBoxStyle(Slider::TextBoxAbove, false, 50, 20);
    
    functions_A_Combo.setName("Function A");
    addAndMakeVisible(&functions_A_Combo);
    
    functions_B_Combo.setName("Function B");
    addAndMakeVisible(&functions_B_Combo);
    
    fillCombos();
    
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

void WaveshaperAudioProcessorEditor::fillCombos()
{
    int comboSize = WaveshaperAudioProcessor::functions.size();
    for(int i = 0; i < comboSize; ++i)
    {
        functions_A_Combo.addItem(WaveshaperAudioProcessor::functions[i], i+1);
        functions_B_Combo.addItem(WaveshaperAudioProcessor::functions[i], i+1);
    }
        
}
void WaveshaperAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::midnightblue.withMultipliedBrightness(0.5));
               
    g.setColour(Colours::cornflowerblue);
    
    auto labelWidth = 70;
    
    g.drawText("Saturation", saturationSlider.getX() + saturationSlider.getWidth()/2 - labelWidth/2, 10, labelWidth, 20,  Justification::centred);
    g.drawText("Gain", gainSlider.getX() + gainSlider.getWidth()/2 - labelWidth/2, getHeight()/2 - labelWidth/4, labelWidth, 20, Justification::centred);
    g.drawText("Symmetry", symmetrySlider.getX() + symmetrySlider.getWidth()/2 - labelWidth/2, 10, labelWidth, 20, Justification::centred);
    
    
    

}

void WaveshaperAudioProcessorEditor::resized()
{
    
    auto box = getLocalBounds();
    auto bigKnob = box.removeFromRight(getWidth()/3);
  

    const auto smallKnobW = getWidth()/4;
    
    saturationSlider.setBounds(bigKnob.reduced(10,10));
    
    auto smallKnobsArea = box.removeFromRight(getWidth()/3);
    
    gainSlider.setBounds(smallKnobsArea.removeFromBottom(smallKnobW));
    symmetrySlider.setBounds(smallKnobsArea.reduced(10));
    
//    auto box = getLocalBounds().reduced(20);
//    gainSlider.setBounds(box.removeFromRight(70).removeFromBottom(120));
//    saturationSlider.setBounds(box.removeFromLeft(70).removeFromBottom(120));
//    symmetrySlider.setBounds(box.removeFromBottom(120));
//    crossfadeSlider.setBounds(box.removeFromLeft(170).removeFromTop(120));
    functions_A_Combo.setBounds(10, 10, getWidth()/3, 50);
    functions_B_Combo.setBounds(getWidth()/2, 10, getWidth()/3, 50);
}

