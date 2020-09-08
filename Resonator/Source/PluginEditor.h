/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ResonLookAndFeel.h"


//==============================================================================
/**
*/

class ResonatorAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                       public juce::Button::Listener
{
public:
    ResonatorAudioProcessorEditor (ResonatorAudioProcessor&);
    ~ResonatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void buttonClicked(juce::Button*) override;
    void buttonStateChanged(juce::Button*) override;
    
private:
    
    ResonatorAudioProcessor& audioProcessor;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    // GUI Objects
    ResonLookAndFeel resonLookAndFeel;
    enum SliderObject {freqSlider, fineTuneSlider, qSlider, gainSlider, numOfSliders};
    //enum Labels {freqLabel, fineLabel, qLabel, gainLabel, numOfLabels};
    enum GuiFrame {freqFrame = 0, fineFrame, qFrame, gainFrame, algorithmListFrame, linkButtonsFrame, numOfFrames};
    enum LinkObject {soundcloud, linkedIn, github, facebook};
    std::vector<std::unique_ptr<juce::Slider>> sliderArray;
    std::vector<std::unique_ptr<juce::Label>> labelArray;
    std::vector<std::unique_ptr<juce::ImageButton>> linkButtonArray;
    juce::ComboBox algoBox;
    juce::TextButton bypassButton;
    
    void initialiseSliders();
    void initialiseImageButtons();
    //void initialiseLabels();
    void fillAlgoBox();
    
    // Size parameters
    const float edge{5.0f};
    const float segmentLength{90.0f};
    const float textBoxWidth{50.0f};
    const float textBoxHeight{25.0f};
    std::vector<std::unique_ptr<juce::Rectangle<int>>> frames;
    void setFrames();
    
    // AudioProcessorValueTreeState attachments
    std::vector<std::unique_ptr<SliderAttachment>> sliderAttachments;
    void attachParameters();
    std::unique_ptr<ComboBoxAttachment> algoBoxAttachment;
    std::unique_ptr<ButtonAttachment> bypassAttachment;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResonatorAudioProcessorEditor)
};

