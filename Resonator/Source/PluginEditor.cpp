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
    //initialiseLabels();

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

void ResonatorAudioProcessorEditor::initialiseImageButtons()
{
    
}

//void ResonatorAudioProcessorEditor::initialiseLabels()
//{
//    labelArray.clear();
//    labelArray.resize(numOfLabels);
//    std::fill(labelArray.begin(), labelArray.end(), nullptr);
//    labelArray[freqLabel] = std::make_unique<juce::Label>("FreqLabel", "FRe<Q>ueNCy");
//    labelArray[fineLabel] = std::make_unique<juce::Label>("FineLabel", "FINe");
//    labelArray[qLabel] = std::make_unique<juce::Label>("qLabel", "ReSONaNCe");
//    labelArray[gainLabel] = std::make_unique<juce::Label>("gainLabel", "GaIN");
//    for (int i = 0; i < labelArray.size(); ++i)
//    {
//        if (labelArray[i] != nullptr)
//        {
//            addAndMakeVisible(*labelArray[i]);
//            labelArray[i]->setAlwaysOnTop(true);
//        }
//    }
//}

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
    
    g.setColour (juce::Colours::gold.withBrightness(0.26f).withHue(1.72f));
    
    setFrames();
    for (int i = 0; i < frames.size(); ++i)
    {
        g.drawRoundedRectangle(frames[i]->toFloat(), 7, 2);
    }
    
    // add lables
    //g.setFont(juce::Font("", "Monaco", 21.0f));
    g.setColour(resonLookAndFeel.getRimColour());
    g.drawFittedText("FRe<Q>ENCy", frames[freqFrame]->reduced(edge * 2), juce::Justification::topRight, 1);
    g.drawFittedText("ReSONANCe", frames[qFrame]->withLeft(frames[qFrame]->getX() - edge * 4).reduced(0, edge * 3), juce::Justification::topLeft, 1);
    g.drawFittedText("FINe", frames[fineFrame]->withRight(getWidth() * 0.67).withBottom(frames[fineFrame]->getBottom() - edge * 2), juce::Justification::bottomRight, 1);
    g.drawFittedText("GaIN", frames[gainFrame]->withTop(frames[freqFrame]->getBottom() - edge), juce::Justification::topRight, 1);
    
}

void ResonatorAudioProcessorEditor::resized()
{
    setFrames();
    
    // resize sliders
    for (int i = 0; i < sliderArray.size(); ++i)
    {
        sliderArray[i]->setBounds(frames[i]->reduced(edge * 4));
    }
    
    // resize combobox
    algoBox.setBounds(frames[algorithmListFrame]->reduced(edge, edge * 3.5f));
    
//    const std::pair<int, int> smallLabelDim = std::make_pair<int, int>(getWidth()/4, getHeight()/10);
//    labelArray[fineLabel]->setBounds(frames[fineFrame]->getBottom() - segmentLength * 0.25, getWidth() * 0.5, smallLabelDim.first, smallLabelDim.second);
    
}

void ResonatorAudioProcessorEditor::setFrames()
{
    frames.clear();
    frames.resize((int)GuiFrame::numOfFrames);
    auto upperHeight = getHeight() * 3 / 10;
    auto lowerHeight = upperHeight;
    auto midSection = getLocalBounds();
    auto upperSection = midSection.removeFromTop(upperHeight);
    auto lowerSection = midSection.removeFromBottom(lowerHeight);
    
    frames[freqFrame] = std::make_unique<juce::Rectangle<int>>(midSection);
    frames[fineFrame] = std::make_unique<juce::Rectangle<int>>(upperSection.removeFromLeft(getWidth()/2));
    frames[qFrame] = std::make_unique<juce::Rectangle<int>>(upperSection);
    frames[gainFrame] = std::make_unique<juce::Rectangle<int>>(lowerSection.removeFromRight(getWidth()/2));
    frames[algorithmListFrame] = std::make_unique<juce::Rectangle<int>>(lowerSection.removeFromTop(lowerSection.getHeight() * 0.5f));
    frames[linkButtonsFrame] = std::make_unique<juce::Rectangle<int>>(lowerSection);

    for (int i = 0; i < frames.size(); ++i)
    {
        assert (frames[i] != nullptr);
    }
   
}

void ResonatorAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    
}

void ResonatorAudioProcessorEditor::buttonStateChanged(juce::Button* button)
{
    
}
