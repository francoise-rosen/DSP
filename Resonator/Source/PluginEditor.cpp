/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ResonatorAudioProcessorEditor::ResonatorAudioProcessorEditor (ResonatorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    setLookAndFeel(&resonLookAndFeel);
    initialiseSliders();

    addAndMakeVisible(&algoBox);
    fillAlgoBox();
    
    bypassButton.setButtonText("On");
    bypassButton.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(&bypassButton);
    
    attachParameters();
    
    setResizable(true, true);
    setResizeLimits(segmentLength, segmentLength * 5 / 3, segmentLength * 6,  segmentLength * 10);
    setSize (segmentLength * 3, segmentLength * 5);
}

ResonatorAudioProcessorEditor::~ResonatorAudioProcessorEditor()
{
}

//==============================================================================

void ResonatorAudioProcessorEditor::fillAlgoBox()
{
    // make a list of available band pass algorithms
    assert (ResonatorAudioProcessor::listOfAlgorithms.size() == (int)sfd::FilterAlgorithm::numOfAlgorithms);
    for (int i = 0; i < (int)sfd::FilterAlgorithm::numOfAlgorithms; ++i){
        algoBox.addItem(ResonatorAudioProcessor::listOfAlgorithms[i], 100 + i);
    }
}

void ResonatorAudioProcessorEditor::initialiseSliders()
{
    for (int i = 0; i < numOfSliders; ++i)
    {
        sliderArray.push_back(std::make_unique<juce::Slider>(
            juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow
                                                                 )
                                  );
        sliderArray[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
        sliderArray[i]->setLookAndFeel(&resonLookAndFeel);
        
        addAndMakeVisible(*(sliderArray[i]));
    };
    
}
                              
void ResonatorAudioProcessorEditor::attachParameters()
{
    // attach the sliders to the AudioProcessorValueTreeState object
    for (int i = 0; i < sliderArray.size(); ++i)
    {
        sliderAttachments.resize(sliderArray.size());
        sliderAttachments[i] = std::make_unique<SliderAttachment>(
            audioProcessor.getValueTree(),
             paramData::paramArray[i].getID(),
             *(sliderArray[i])
                                                                  );
    }
    
    // attach the combobox to the AudioProcessorValueTreeState object
    algoBoxAttachment = std::make_unique<ComboBoxAttachment>(
         audioProcessor.getValueTree(), paramData::paramArray[paramData::algo].getID(), algoBox
                                                             );
    // attach the bypass button to the AudioProcessorValueTreeState object
    bypassAttachment = std::make_unique<ButtonAttachment>(
         audioProcessor.getValueTree(), paramData::paramArray[paramData::bypass].getID(), bypassButton
                                                          );
    
}

//==============================================================================
void ResonatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    resonLookAndFeel.setBackgroundColour(juce::Colours::black);
    
    g.setColour (juce::Colours::gold);
    
    setFrames();
    for (int i = 0; i < frames.size(); ++i)
    {
        if (frames[i] != nullptr)
        {
           g.drawRoundedRectangle(frames[i]->toFloat(), 7, 2);
        }
    }

}

void ResonatorAudioProcessorEditor::resized()
{

    setFrames();
    
    for (int i = 0; i < sliderArray.size(); ++i)
    {
        sliderArray[i]->setBounds(frames[i]->reduced(edge * 4));
    }
}

void ResonatorAudioProcessorEditor::setFrames()
{
    frames.resize((int)GuiFrame::numOfFrames);
    auto upperHeight = getHeight() * 3 / 10;
    auto lowerHeight = upperHeight;
    auto midSection = getLocalBounds();
    auto upperSection = midSection.removeFromTop(upperHeight);
    auto lowerSection = midSection.removeFromBottom(lowerHeight);
    
    frames[static_cast<int>(GuiFrame::freqControl)] = std::make_unique<juce::Rectangle<int>>(midSection);
    frames[(int)GuiFrame::fineControl] = std::make_unique<juce::Rectangle<int>>(upperSection.removeFromLeft(getWidth()/2));
    frames[(int)GuiFrame::qControl] = std::make_unique<juce::Rectangle<int>>(upperSection);
    frames[(int)GuiFrame::gainControl] = std::make_unique<juce::Rectangle<int>>(lowerSection.removeFromRight(getWidth()/2));
    frames[(int)GuiFrame::socialAndLogoObjects] = std::make_unique<juce::Rectangle<int>>(lowerSection);
    
    // check if there're no nullptr in the vector
    for (int i = 0; i < frames.size(); ++i)
    {
        assert (frames[i] != nullptr);
    }
   
    
}
