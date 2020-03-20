/*
  ==============================================================================

    TransferFunction.h
    Created: 5 Feb 2020 8:13:32pm
    Author:  syfo_dias

*/

#pragma once
#define MIN 0.0
#define MAX 1.0
#define MINRANGE -1.0
#define MAXRANGE 1.0
#define HALFPI MathConstants<double>::halfPi

template <typename T>
T linearScale(T& valueToScale, const T inMin, const T inMax, const T outMin, const T outMax)
{
    T value;
    jassert(inMin < inMax);
    if(outMin == outMax) return outMin;
    
    const T factor = (outMax - outMin) / (inMax - inMin);
    value = factor * (valueToScale - inMin) + outMin;
    
    return value;
}

template<typename T>
T expScale(T& valueToScale, const T inMin, const T inMax, const T outMin, const T outMax, const T expFactor)
{
    // expFactor in range (1, n) will produce exponential growth
    // expFactor in range (0, 1) will produce logarithmic growth
    // expFactor 1 is simple linearScale;
    jassert(expFactor >= 0);
    jassert(valueToScale >= 0); // we don't need negative numbers here
    
    T value = valueToScale;
    if(expFactor == static_cast<T>(1)) return linearScale(valueToScale, inMin, inMax, outMin, outMax);
    
    // assume the input range is 0 - 1 (otherwise we may need to scale it first
    if(!(inMin == static_cast<T>(MIN) && inMax == static_cast<T>(MAX)))
    {
        T localMin = static_cast<T>(MIN);
        T localMax = static_cast<T>(MAX);
        value = linearScale(value, inMin, inMax, localMin, localMax);
    }
    
    value = pow(value, expFactor);
    value = linearScale(value, static_cast<T>(MIN), static_cast<T>(MAX), outMin, outMax);
    return value;
}



inline double sgn(double inpt)
{
    return (inpt >= 0) ? 1 : -1;
}

class TransferFunction
{
public:
    
    TransferFunction()
    :min(MIN), // no modulation
    max(MAX),// max modulation
    currentFunction(Functions::tanh)
    {}

    
    enum Functions
    {
        tanh,
        cos,
        sfdSine,
        softClipper,
        softClipperCascade3,
        numFunctions
    };
    

    // this one is too quiet, why?
    forcedinline double halfCos(double& x, double index, double sym=0) noexcept
    {
        double localIndex = expScale(index, min, max, 1.0, 2.0, 3.0);
        double threshold = HALFPI * localIndex;
        double sample = linearScale(x, MINRANGE, MAXRANGE, -threshold, threshold);
        // deal with symmetry
        
        sample = std::cos(sample + HALFPI);
        return sample;
    }
    
    // this one nicely grows to distortion
    forcedinline double sfdsine(double& x, double index, double sym=0) noexcept
    {
        index = expScale(index, min, max, 0.1, 10.0, 4.0);
        double sample = linearScale(x, MINRANGE, MAXRANGE, -HALFPI, HALFPI);

        sample = std::sin(
                          sample + std::sin(
                                            sample * index + sym * std::cos (
                                                                            sample * index
                                                                             )
                                            )
                          );
        return sample;
    }
    

    forcedinline double sfdtanh(double& x, double index, double sym=1) noexcept
    {
        // processing
        index = expScale(index, min, max, 1.0, 5.0, 2.0);
        auto sample = std::tanh(x * index) / std::tanh(index * 2.0);
        return sample;
    }
    
    forcedinline double sfdatan(double& x, double index, double sym=1) noexcept
    {
        index = expScale(index, min, max, 1.0, 9.0, 0.75);
        auto sample = std::atan(x * index) / std::atan(index * 2.0);
        return sample;
    }
    
    
    // use static?
    forcedinline double sfdClip(double x, double index, double sym=1) noexcept
    {
        
        index = expScale(index, min, max, 0.75, 1.25, 0.75);
        double sample = x * index;
        if(std::abs(sample) > 1) sample = sgn(sample) * 2/3;
        if(std::abs(sample) <= 1) sample -= (std::pow(sample, 3) / 3); // wrong for negative values?
        
        // normalise
        sample *= 3 / 2;
      
        return sample;
    }
    
    // experiment with coefs here!
    forcedinline double sfdClipCascade3f(double x, double index, double sym=1) noexcept
    {
        double sample = sfdClip(
                            sfdClip(
                                sfdClip(x, index * 0.99998),
                                                index * 0.49998),
                                                    index * 0.1999998);
        
        return sample;
    };
    
   
    
    forcedinline double transform(Functions func, double& x, double index, double sym)
    {
        double sample;
        switch(func)
        {
            case tanh:
                sample = sfdtanh(x, index, sym);
                std::cout << "tanh called\n";
                break;
            case cos:
                sample = halfCos(x, index, sym);
                std::cout << "cos called\n";
                break;
            case sfdSine:
                sample = sfdsine(x, index, sym);
                std::cout << "sine called\n";
                break;
            case softClipper:
                sample = sfdClip(x, index, sym);
                std::cout << "clip called\n";
                break;
            case softClipperCascade3:
                sample = sfdClipCascade3f(x, index, sym);
                std::cout << "clip cascade called\n";
                break;
            default:
                return x;
                
        }
        
        return sample;
    }
    

private:
    double min;
    double max;
    Functions currentFunction;

    
};
