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

    setSize (500, 290);
    gainSlider.setName("Gain");
    addAndMakeVisible(&gainSlider);
    gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);

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
    crossfadeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
    
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
  
    g.fillAll (Colours::midnightblue.withMultipliedBrightness(0.7));
               
    g.setColour(Colours::cornflowerblue);
    
    auto labelWidth = 70;
    
    g.drawText("Saturation", saturationSlider.getX() + saturationSlider.getWidth()/2 - labelWidth/2, gap, labelWidth, 20,  Justification::centred);
    g.drawText("Symmetry", symmetrySlider.getX() + symmetrySlider.getWidth()/2 - labelWidth/2, getHeight()/1.75 - labelWidth/4 + gap, labelWidth, 20, Justification::centred);
    g.drawText("Gain", gainSlider.getX() + gainSlider.getWidth()/2 - labelWidth/2, gainSlider.getY() - labelWidth/4, labelWidth, 20, Justification::centred);
    
    g.setColour(Colours::yellow);
    
    Rectangle<float> area (gap, gap, getWidth()/4, getHeight() - gap - gap);
    area.withTrimmedLeft(10);
    g.drawRect(area);
    
    g.setColour(Colours::greenyellow);
    auto box = getLocalBounds().reduced(gap);
    g.drawRect(box);
    
    g.setColour(Colours::yellowgreen);
    auto crossArea = getLocalBounds().removeFromTop(getHeight()/2).removeFromRight(getWidth() * 3 / 4).reduced(gap);
    g.drawRect(crossArea);
    
    

}

void WaveshaperAudioProcessorEditor::resized()
{
    
    auto gainArea = getLocalBounds().removeFromRight(getWidth()/4).removeFromBottom(getHeight()/2);
    auto modArea = getLocalBounds().removeFromLeft(getWidth()/4);
    auto crossfadeArea = getLocalBounds().removeFromTop(getHeight()/2).removeFromRight(getWidth() * 3 / 4);
    
    
    saturationSlider.setBounds(modArea.removeFromTop(getHeight()/1.75).reduced(gap));
    symmetrySlider.setBounds(modArea.reduced(gap));
    
    gainSlider.setBounds(gainArea.reduced(20));
    
    crossfadeSlider.setBounds(crossfadeArea.removeFromTop(getHeight()/4).reduced(gap));
    
    auto comboArea = crossfadeArea.reduced(gap, 0).removeFromTop(crossfadeArea.getHeight() * 2 / 3);
    functions_A_Combo.setBounds(comboArea.removeFromLeft(getWidth()/4).reduced(gap));
    functions_B_Combo.setBounds(comboArea.removeFromRight(getWidth()/4).reduced(gap));
    
    
}

