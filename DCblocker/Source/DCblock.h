/*
  ==============================================================================

    DCblock.h
    Created: 23 Mar 2020 3:54:22pm
    Author:  syfo_dias
    
    Simple one zero / one pole topology for a DCblocker filter
    Zero is constant 1 and so omitted 
    Pole for DCblock object must be set in range 0.995-0.997, 
    otherwise it's just a HPF

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#define DCBID "DCblockID"
#define DCBNAME "DCblockName"

class DCblock
{
public:
    DCblock();
    
    void setPole(double& p);
    
    float process(float inputSample) noexcept;
    

private:
    //static constexpr double pole{0.995};
    float x;
    float y;
    double pole;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DCblock)
};
