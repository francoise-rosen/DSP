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
    const float gap{10};

    WaveshaperAudioProcessor& processor;
    
    // connection between slider and parameter in tree state
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> saturationSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> symmetrySliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> crossfadeSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> functions_A_ComboAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> functions_B_ComboAttachment;
    
     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveshaperAudioProcessorEditor)
};
