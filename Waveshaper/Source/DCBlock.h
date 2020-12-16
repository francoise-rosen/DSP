
/*
  ==============================================================================

    DCblock.h
    Created: 23 Mar 2020 3:54:22pm
    Author:  syfo_dias
    
    Simple one zero / one pole topology for a DCblocker filter
    Zero is constant 1 and so omitted 
    Pole for DCblock object must be set in range 0.995-0.997, 
    otherwise it's just a high pass filter

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class DCblock
{
public:
    DCblock()
    :x{0}, y{0} {}

    float process(float inputSample) noexcept
    {
    float output = inputSample - x + y * pole;
    x = inputSample; // xz-1
    y = output;      // yz-1
    return output;
}
    
private:
    float x;
    float y;
    static constexpr float pole{0.997f};
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DCblock)
};
