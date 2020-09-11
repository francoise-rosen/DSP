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
    initialiseImageButtons();

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
    setLookAndFeel(nullptr);
}

//==============================================================================

void ResonatorAudioProcessorEditor::fillAlgoBox()
{
    // make a list of available band pass algorithms
    assert (ResonatorAudioProcessor::listOfAlgorithms.size() == static_cast<int>(sfd::FilterAlgorithm::numOfAlgorithms));
//    for (int i = 0; i < (int)sfd::FilterAlgorithm::numOfAlgorithms; ++i){
//        algoBox.addItem(ResonatorAudioProcessor::listOfAlgorithms[i], 100 + i);
//    }
    algoBox.addItemList(ResonatorAudioProcessor::listOfAlgorithms, 100);
    algoBox.setJustificationType(juce::Justification::centred);
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
    imageButtonArray.clear();
    
    imageButtonArray.resize(numOfImageButtons);
    
    // github, instagram, linkedin, soundcloud
    std::array<juce::Image, numOfImageButtons> images {
        juce::ImageCache::getFromMemory(BinaryData::github_square_black_png, BinaryData::github_square_black_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::linkedin_square_black_png, BinaryData::linkedin_square_black_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::soundcloud_square_black_png, BinaryData::soundcloud_square_black_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::instagram_square_black_png, BinaryData::instagram_square_black_pngSize)
    };
    
    juce::StringArray componentIDs {
        "https://github.com/francoise-rosen",
        "https://www.linkedin.com/in/stan-barvinsky/",
        "https://soundcloud.com/francoise_rosen",
        "https://www.instagram.com/francoise.rosen/?hl=en"
    };
    
    assert (images.size() == imageButtonArray.size());
    
    for (int i = 0; i < imageButtonArray.size(); ++i)
    {
        imageButtonArray[i] = std::make_unique<juce::ImageButton>();
        imageButtonArray[i]->setImages(false, true, true, images[i], 1.0f, juce::Colours::white.withAlpha(0.75f), images[i], 1.0f, juce::Colours::white.withAlpha(0.25f), images[i], 1.0f, juce::Colours::lightgrey);
        imageButtonArray[i]->setComponentID(componentIDs[i]);
        addAndMakeVisible(*imageButtonArray[i]);
        imageButtonArray[i]->addListener(this);
    }

}

void ResonatorAudioProcessorEditor::attachParameters()
{
    // attach the sliders to the AudioProcessorValueTreeState object
    sliderAttachments.clear();
    sliderAttachments.resize(sliderArray.size());
    
    for (int i = 0; i < sliderArray.size(); ++i)
    {
        sliderAttachments[i] = std::make_unique<SliderAttachment>(
            audioProcessor.getValueTree(),
             paramData::paramArray[i].getID(),
             *(sliderArray[i])
                                                                  );
    }
    
    // attach the combobox to the AudioProcessorValueTreeState object
    algoBoxAttachment = std::make_unique<ComboBoxAttachment>(
         audioProcessor.getValueTree(),
             paramData::paramArray[paramData::algo].getID(),
                 algoBox
                                                             );
    // attach the bypass button to the AudioProcessorValueTreeState object
    bypassAttachment = std::make_unique<ButtonAttachment>(
         audioProcessor.getValueTree(),
             paramData::paramArray[paramData::bypass].getID(),
                 bypassButton
                                                          );
    
}

//==============================================================================
void ResonatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Colour backgroundColour = juce::Colours::black;
    g.fillAll (backgroundColour);
    resonLookAndFeel.setBackgroundColour(backgroundColour);
    
    g.setColour (juce::Colours::gold.withBrightness(0.26f).withHue(1.72f));
    
    if (frames.size() != numOfFrames)
        setFrames();
    
    for (int i = 0; i < frames.size(); ++i)
    {
        g.drawRoundedRectangle(frames[i]->toFloat(), 7, 2);
        if (frames[i] == frames[linkButtonsFrame])
        {
            g.setColour(backgroundColour.withAlpha(0.5f));
            g.fillRect(*frames[linkButtonsFrame]);
            g.setColour(juce::Colours::gold.withBrightness(0.26f).withHue(1.72f));
        }
    }
    
    // add lables
    juce::Rectangle<int> resLabelRect {getWidth()/3, static_cast<int>(edge), getWidth()/3, static_cast<int>(edge * 2 + fontHeight)};
    juce::Rectangle<int> fineLabelRect {getWidth()/3, static_cast<int>(frames[fineFrame]->getHeight() * 2.0f / 3.0f), static_cast<int>(frames[fineFrame]->getWidth() / 3.0f), resLabelRect.getHeight()};
    juce::Rectangle<int> gainLabelRect {frames[gainFrame]->withTop(frames[freqFrame]->getBottom() - fontHeight / 2).withRight(frames[gainFrame]->getRight() - edge * 2)};
    
    g.setFont(juce::Font("Monaco", "Bold", fontHeight));
    g.setColour(resonLookAndFeel.getRimColour());
    g.drawFittedText("Fre(Q)ueNCy", frames[freqFrame]->reduced(edge * 2), juce::Justification::topRight, 1);
    g.drawFittedText("ReSONaNCe", resLabelRect, juce::Justification::centred, 1);
    g.drawFittedText("FINe", fineLabelRect, juce::Justification::centred, 1);
//    g.drawFittedText("GaIN", frames[gainFrame]->withTop(frames[freqFrame]->getBottom() - edge), juce::Justification::topRight, 1);
    g.drawFittedText("GaIN", gainLabelRect, juce::Justification::topRight, 1);
    
    // freq line
    g.setColour(resonLookAndFeel.getRimColour().withAlpha(0.5f));
    juce::Path freqLinePath;
    std::vector<juce::Point<float>> freqLinePoints
    {
        // Point 0
        {frames[freqFrame]->getWidth() * 0.67f,
            frames[freqFrame]->getY() + edge * 3 + fontHeight},
        // Point 1
        {frames[freqFrame]->getWidth() * 0.69f,
            frames[freqFrame]->getY() + edge * 3 + fontHeight},
        // Point 2
        {frames[freqFrame]->getWidth() * 0.95f,
            frames[freqFrame]->getY() + edge * 3 + fontHeight},
        // Point 3
        {getWidth() * 0.72f,
            frames[freqFrame]->getY() + edge * 5 + fontHeight},
        // Point 4
        {getWidth() * 0.75f,
            frames[freqFrame]->getY() + edge * 7 + fontHeight},
        // Point 5
        {getWidth() * 0.7f,
            frames[freqFrame]->getY() + edge * 7 + fontHeight}
        
    };

    freqLinePath.startNewSubPath(freqLinePoints[2]);
    freqLinePath.cubicTo(freqLinePoints[1], freqLinePoints[0], freqLinePoints[3]);
    freqLinePath.quadraticTo(freqLinePoints[4], freqLinePoints[5]);
    freqLinePath.createPathWithRoundedCorners(18.0f);
    g.strokePath(freqLinePath, juce::PathStrokeType(1.0f));
    
    // fine line

}

void ResonatorAudioProcessorEditor::resized()
{
    setFrames();
    fontHeight = getHeight() / 30.0f;
    
    // resize sliders
    for (int i = 0; i < sliderArray.size(); ++i)
    {
        sliderArray[i]->setBounds(frames[i]->reduced(edge * 4));
    }
    
    // resize combobox
    algoBox.setBounds(frames[algorithmListFrame]->reduced(edge * 2.0f, edge * 3.0f));
    //algoBox.setBounds(frames[algorithmListFrame]->reduced(edge, edge * 1.5f));
    
    // buttons
    auto buttonArea = *frames[linkButtonsFrame];
    buttonArea.reduced(edge * 2);
    auto buttonWidth = buttonArea.getWidth() / numOfImageButtons;
    for (int i = 0; i < imageButtonArray.size(); ++i)
    {
        imageButtonArray[i]->setBounds(buttonArea.removeFromLeft(buttonWidth).reduced(edge * 1.5, edge * 2));
    }
    
    repaint(); // resizes the font height
    
}

void ResonatorAudioProcessorEditor::setFrames()
{
    frames.clear();
    frames.resize(numOfFrames);
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

    juce::URL link {button->getComponentID()};
    
    if (link.isWellFormed())
    {
        link.launchInDefaultBrowser();
    }
    
}

void ResonatorAudioProcessorEditor::buttonStateChanged(juce::Button* button)
{
    
}
