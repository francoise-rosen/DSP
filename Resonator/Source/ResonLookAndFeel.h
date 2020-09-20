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
        setColour(juce::Slider::textBoxOutlineColourId, colourSet[backgroundColour].withAlpha(0.0f));
        setColour(juce::ComboBox::outlineColourId, colourSet[backgroundColour].darker());
        setColour(juce::ComboBox::backgroundColourId, colourSet[guiObjectColour]);
        setColour(juce::PopupMenu::backgroundColourId, colourSet[backgroundColour].withAlpha(0.4f));
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
        
        // this is the vertical surface of the slider
        g.setColour(colourSet[backgroundColour]);
        g.drawEllipse(rx, ry, rw, rw, rimThickness);
        
        // this is the outer-bottom rim
        g.setColour(colourSet[textAndRimColour].darker());
        float outerRimOffset = 3.0f;
        g.drawEllipse(rx - rimThickness + outerRimOffset, ry - rimThickness + outerRimOffset, rw + rimThickness, rw + rimThickness, outerRimOffset);
        
        // draw the thumb
        drawSliderThumb(g, radius, centreX, centreY, angle);
        
    }
    
    // set combo box font
    juce::Font getComboBoxFont(juce::ComboBox& box) override
    {
        return currentComboBoxFont();
    }
    
    // set combo box popup font
    
    juce::Font getPopupMenuFont() override
    {
        return currentComboBoxFont();
    }
    
    void drawComboBox(juce::Graphics& g, int width, int height, bool down, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box) override
    {
        auto boxArea = box.getLocalBounds();

        g.setColour(colourSet[guiObjectColour].darker());
        boxArea.reduced(width * 0.1f, height * 0.2f);
        fontHeight = boxArea.getHeight() * 0.35f;
        g.fillRoundedRectangle(boxArea.toFloat(), height * 0.05f);

        // draw the drop-down arrow
        g.setColour(colourSet[backgroundColour]);
        juce::Path dropDownArrow;
        dropDownArrow.startNewSubPath(boxArea.getRight() - boxArea.getWidth() * 0.2f, boxArea.getY() + boxArea.getHeight() * 0.45f);
        dropDownArrow.lineTo(boxArea.getRight() - boxArea.getWidth() * 0.15f, boxArea.getY() + boxArea.getHeight() * 0.55f);
        dropDownArrow.lineTo(boxArea.getRight() - boxArea.getWidth() * 0.1f, boxArea.getY() + boxArea.getHeight() * 0.45f);
        dropDownArrow.closeSubPath();
        g.fillPath(dropDownArrow);
        
        // outer rim
        auto rim = boxArea.reduced(3.0f);
        g.setColour(colourSet[backgroundColour].withAlpha(0.5f));
        g.drawRoundedRectangle(rim.toFloat(), height * 0.05f, 5.0f);
        

    }
    
    juce::Colour getBackgroundColour() const
    {
        return colourSet[backgroundColour];
        
    }
    juce::Colour getRimColour() const
    {
        return colourSet[textAndRimColour];
        
    }
    
    void setBackgroundColour(juce::Colour colour)
    {
        colourSet[backgroundColour] = colour;
    }
    
    
private:
    enum {backgroundColour, textAndRimColour, guiObjectColour, numOfColours};
    std::array<juce::Colour, numOfColours> colourSet {juce::Colours::black, juce::Colours::whitesmoke, juce::Colours::darkblue};
    
    void drawSliderThumb(juce::Graphics& g, const float& radius, const float& centreX, const float& centreY, const float& angle)
    {
        juce::Path p;
        
        // shape
        float sliderThumbLength = radius * 0.57f;
        float sliderThumbWidth = sliderThumbLength * 0.1f;
        p.addRoundedRectangle(-sliderThumbWidth * 0.5f, -radius, sliderThumbWidth, sliderThumbLength, 3.0f, 3.0f, true, true, false, true);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
        
        g.fillPath(p);
        
    }
    
    int fontHeight {14};
    
    juce::Font currentComboBoxFont()
    {
        return juce::Font("Monaco", "Plain", fontHeight);
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResonLookAndFeel)
    
};



