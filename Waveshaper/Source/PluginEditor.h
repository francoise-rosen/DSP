/*
  ==============================================================================

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class WaveshaperAudioProcessorEditor  : public AudioProcessorEditor

{
public:
    WaveshaperAudioProcessorEditor (WaveshaperAudioProcessor&);
    ~WaveshaperAudioProcessorEditor();

    //==============================================================================
    void fillCombos();
    void paint (Graphics&) override;
    void resized() override;
    

private:
 
    Slider gainSlider;
    Slider saturationSlider;
    Slider symmetrySlider;
    ComboBox functions_A_Combo;
    ComboBox functions_B_Combo;
    Slider crossfadeSlider;
    
    Label dBLabel; // do I need labels or I just use fittedText? 

    double level = 0.0;
    
    
    
    WaveshaperAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveshaperAudioProcessorEditor)
    
    // connection between slider and parameter in tree state
    // if this pointer deleted, the connection brakes
    // don't delete the slider object or AudioProcessorTreeState object before this one!
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> saturationSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> symmetrySliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> crossfadeSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> functions_A_ComboAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> functions_B_ComboAttachment;
};
