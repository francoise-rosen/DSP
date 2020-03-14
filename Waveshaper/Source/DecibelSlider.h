/*
  ==============================================================================

    DecibelSlider.h
    Created: 25 Jan 2020 4:40:47pm
    Author:  syfo_dias
 
    Taken from the JUCE page
 
 This is redundant

  ==============================================================================
*/

#pragma once

// this how you customise your GUI
class DecibelSlider : public Slider
{
public:
    DecibelSlider() {}
    
    double getValueFromText(const String& text) override
    {
        auto infinity_dB = -100.0;
        // first arg is substring to end with
        // second - include substring
        // third ignore case
        // trim() copy string with any whitespace characters removed from the start
        // and the end
        auto decibelText = text.upToFirstOccurrenceOf("dB", false, false).trim();
        // does decibelText matches "-INF"? return -100 else get double out of string
        return decibelText.equalsIgnoreCase("-INF") ? infinity_dB : decibelText.getDoubleValue();
    }
    
    String getTextFromValue(double value) override
    {
        return Decibels::toString(value);
    }
private:
      JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DecibelSlider)
};
