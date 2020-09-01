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
    
    // GUI Objects
    enum slider {freqSlider, fineTuneSlider, qSlider, gainSlider, numOfSliders};
    void initialiseSliders();

    std::vector<std::unique_ptr<juce::Slider>> sliderArray;
    juce::ComboBox algoBox;
    juce::TextButton bypassButton;
    
    void fillAlgoBox();
    
    const int textBoxWidth {55};
    const int textBoxHeight {10};
    int numOfAreas {6};
    
    
    std::vector<std::unique_ptr<SliderAttachment>> sliderAttachments;
    void attachParameters();
    
    std::unique_ptr<ComboBoxAttachment> algoBoxAttachment;
    std::unique_ptr<ButtonAttachment> bypassAttachment;
    
    const int edge{5};
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResonatorAudioProcessorEditor)
};

