/*
  ==============================================================================

    ResonLookAndFeel.h
    Created: 6 Sep 2020 12:29:23pm
    Author:  syfo_dias

  ==============================================================================
*/

#pragma once
#include <array>

class ResonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ResonLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, colourSet[textAndRimColour]);
        setColour(juce::Slider::textBoxOutlineColourId, colourSet[backgroundColour]);
    }
    
    ~ResonLookAndFeel() {}
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosition, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {
        
        float rimThickness = static_cast<float> (juce::jmin(width * 0.5f, height * 0.5f)) * 0.25f; // 12.5% of diameter
        float radius = static_cast<float> (juce::jmin(width * 0.5f, height * 0.5f)) - rimThickness;
        float centreX = static_cast<float>(x) + static_cast<float>(width) * 0.5f;
        float centreY = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
        float rx = centreX - radius;
        float ry = centreY - radius;
        float rw = radius * 2.0f;
        float angle = rotaryStartAngle + sliderPosition * (rotaryEndAngle - rotaryStartAngle);
        
        g.setColour(colourSet[guiObjectColour].darker());
        g.fillEllipse(rx, ry, rw, rw);
        g.setColour(colourSet[backgroundColour]);
        g.drawEllipse(rx, ry, rw, rw, rimThickness);
        g.setColour(colourSet[textAndRimColour].darker());
        float outerRimOffset = 3.0f;
        g.drawEllipse(rx - rimThickness + outerRimOffset, ry - rimThickness + outerRimOffset, rw + rimThickness, rw + rimThickness, outerRimOffset);
        
        // draw the thumb
        
    }
    
    void drawThumb()
    {
        juce::Path p;
        
        // shape
       // float thumbLength =
        
        // transform (quadrilateral, triangle, rounded rectangle, rectangle)
    }
    
    juce::Colour getBackgroundColour() const {return colourSet[backgroundColour];}
    void setBackgroundColour(juce::Colour colour)
    {
        colourSet[backgroundColour] = colour;
    }
    
    
    
private:
    enum {backgroundColour, textAndRimColour, guiObjectColour, numOfColours};
    std::array<juce::Colour, numOfColours> colourSet {juce::Colours::black, juce::Colours::whitesmoke, juce::Colours::darkblue};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResonLookAndFeel)
    
};



