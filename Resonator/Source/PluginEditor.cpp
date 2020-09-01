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

    initialiseSliders();

    addAndMakeVisible(&algoBox);
    fillAlgoBox();
    
    bypassButton.setButtonText("On");
    bypassButton.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(&bypassButton);
    
    attachParameters();
    
    setSize (210, 240);
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
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    g.setColour (juce::Colours::silver);
    
    auto leftArea = getLocalBounds().removeFromLeft(getWidth() * 0.65).reduced(edge);
    auto rightArea = getLocalBounds().removeFromRight(getWidth() * 0.35).reduced(edge);
    g.drawRoundedRectangle(leftArea.withRightX(getWidth() * 0.45).withX(edge).toFloat(), 3, 3);
    for (int i = 0; i < 3; ++i) {
        g.drawRoundedRectangle(rightArea.removeFromTop(getHeight()/3).toFloat(), 3, 3);
    }
    
    g.setColour(juce::Colours::whitesmoke);
    g.drawFittedText("FREQ", leftArea.withBottom(getHeight() / 3).withRightX(getWidth() * 0.56), juce::Justification::centredRight, 1);
}

void ResonatorAudioProcessorEditor::resized()
{
    auto leftArea = getLocalBounds().removeFromLeft(getWidth() * 0.5);
    
    auto sliders = 3;
    auto sliderHeight = getHeight()/sliders;
    for (int i = 0; i < sliders; ++i) {
            sliderArray[i]->setBounds(leftArea.removeFromTop(sliderHeight));
        }
    auto rightArea = getLocalBounds().removeFromRight(getWidth()* 0.35);
    sliderArray[paramData::gain]->setBounds(rightArea.removeFromBottom(sliderHeight));
    algoBox.setBounds(rightArea.removeFromBottom(sliderHeight/2).reduced(edge, edge*2));
    bypassButton.setBounds(rightArea.removeFromBottom(sliderHeight/2).removeFromRight(rightArea.getWidth()/4).reduced(edge));
    

    
}
