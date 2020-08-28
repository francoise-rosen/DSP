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
class OnePoleAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    OnePoleAudioProcessorEditor (OnePoleAudioProcessor&);
    ~OnePoleAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    
    OnePoleAudioProcessor& audioProcessor;
    juce::Slider freqSlider;
    juce::Slider gainSlider;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<SliderAttachment> freqSliderAttachment;
    std::unique_ptr<SliderAttachment> gainSliderAttachment;
    
    float textBoxWidth{50};
    float textBoxHeight{20};
    
    std::unique_ptr<int> border;
    int dialWidth;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OnePoleAudioProcessorEditor)
};
