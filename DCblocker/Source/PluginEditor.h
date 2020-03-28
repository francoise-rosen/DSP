/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DcblockerAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    DcblockerAudioProcessorEditor (DcblockerAudioProcessor&);
    ~DcblockerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
   
    DcblockerAudioProcessor& processor;
    Slider dcslider;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> filterSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DcblockerAudioProcessorEditor)
};
