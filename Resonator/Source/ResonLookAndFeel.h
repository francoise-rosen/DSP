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
        g.setColour(colourSet[backgroundColour]);
        g.drawEllipse(rx, ry, rw, rw, rimThickness);
        g.setColour(colourSet[textAndRimColour].darker());
        float outerRimOffset = 3.0f;
        g.drawEllipse(rx - rimThickness + outerRimOffset, ry - rimThickness + outerRimOffset, rw + rimThickness, rw + rimThickness, outerRimOffset);
        
        // draw the thumb
        drawSliderThumb(g, radius, centreX, centreY, angle);
        
    }
    
    // BUTTONS
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& background, bool highlighted, bool down) override
    {
        
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
        boxArea.reduced(width * 0.1f, height * 0.2f);
        
        // draw a "shadow"
        float offset = height * 0.25f;
        g.setColour(colourSet[textAndRimColour].darker());
        g.drawRoundedRectangle(boxArea.withPosition(boxArea.getX() - 20, boxArea.getY() - 20).toFloat(), height * 0.1f, 6.0f);

        g.setColour(colourSet[guiObjectColour].darker());
        fontHeight = boxArea.getHeight() * 0.6f;
        g.fillRoundedRectangle(boxArea.toFloat(), height * 0.25f);
    
        // draw pointer
        

    }
    
    juce::Colour getBackgroundColour() const {return colourSet[backgroundColour];}
    juce::Colour getRimColour() const {return colourSet[textAndRimColour];}
    
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



