/*
  ==============================================================================

    DCblock.cpp
    Created: 23 Mar 2020 3:54:22pm
    Author:  syfo_dias

  ==============================================================================
*/

#include "DCblock.h"



DCblock::DCblock()
:x{0}, y{0}, pole{0.995} // default pole set to blocker
{
}

void DCblock::setPole(double& p)
{
    assert(p >= 0.0 && p <= 1.0);
    pole = p;
}

float DCblock::process(float inputSample) noexcept
{
    // oversample here?
    float output = inputSample - x + y * pole;
    x = inputSample; // xz-1
    y = output;      // yz-1
    return output;
}



