/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ResonatorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ResonatorAudioProcessorEditor (ResonatorAudioProcessor&);
    ~ResonatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ResonatorAudioProcessor& audioProcessor;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    
    juce::Slider freqSlider;
    juce::Slider qSlider;
    juce::Slider gainSlider;
    juce::ComboBox algoBox;
    juce::TextButton bypassButton;
    
    void fillAlgoBox();
    
    int textboxWidth {65};
    int textboxHeight {25};
    
    std::unique_ptr<SliderAttachment> freqSliderAttachment;
    std::unique_ptr<SliderAttachment> qSliderAttachment;
    std::unique_ptr<SliderAttachment> gainSliderAttachment;
    std::unique_ptr<ComboBoxAttachment> algoBoxAttachment;
    std::unique_ptr<ButtonAttachment> bypassAttachment;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResonatorAudioProcessorEditor)
};

