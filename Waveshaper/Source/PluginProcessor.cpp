/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================

// PARAMETER IDs
String WaveshaperAudioProcessor::paramGain{"gain"};
String WaveshaperAudioProcessor::paramSaturation{"saturation"};
String WaveshaperAudioProcessor::paramSymmetry{"symmetry"};
String WaveshaperAudioProcessor::paramCrossfade{"crossfade"};
String WaveshaperAudioProcessor::paramTransferFunctionListA{"tfA"};
String WaveshaperAudioProcessor::paramTransferFunctionListB{"tfB"};

StringArray WaveshaperAudioProcessor::functions{"SFDTanh", "SFDcos", "SFDsine", "SFDclip", "SFDClipCascade"};

//==============================================================================

template <typename T>
T mix(T leftinput, T rightinput, T balance)
{
    return leftinput * balance + rightinput * (1 - balance);
}

//==============================================================================
WaveshaperAudioProcessor::WaveshaperAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),

#endif
        mainGain(0.0),
        parameters(*this,  // add to this processor
                  nullptr, // ?
                  "PARAMETER", // identifier
                  {std::make_unique<AudioParameterFloat> (paramGain,
                                                          GAIN_NAME,
                                                          -100.0f,
                                                          0.0f,
                                                          -100.0f
                                                          ),
                      std::make_unique<AudioParameterFloat> (paramSaturation,
                                                             SATURATION_NAME,
                                                             0.0f,
                                                             1.0f,
                                                             0.0f
                                                             ),
                      std::make_unique<AudioParameterFloat> (paramSymmetry,
                                                             SYMMETRY_NAME,
                                                             -1.0f,
                                                             1.0f,
                                                             0.0f
                                                             ),
                      std::make_unique<AudioParameterFloat> (paramCrossfade,
                                                             CROSSFADE_NAME,
                                                             0.0f,
                                                             1.0f,
                                                             0.5f
                                                             ),
                      std::make_unique<AudioParameterChoice> (paramTransferFunctionListA,
                                                              FUNCTIONS_A_NAME,
                                                              functions,
                                                              0),
                      std::make_unique<AudioParameterChoice> (paramTransferFunctionListB,
                                                              FUNCTIONS_B_NAME,
                                                              functions,
                                                              0)
                  }
                  )

{
}

WaveshaperAudioProcessor::~WaveshaperAudioProcessor()
{
}

//==============================================================================
const String WaveshaperAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WaveshaperAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WaveshaperAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WaveshaperAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WaveshaperAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WaveshaperAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WaveshaperAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WaveshaperAudioProcessor::setCurrentProgram (int index)
{
}

const String WaveshaperAudioProcessor::getProgramName (int index)
{
    return {};
}

void WaveshaperAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void WaveshaperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mainGain = *parameters.getRawParameterValue(paramGain);
    saturation = *parameters.getRawParameterValue(paramSaturation);
    symmetry = *parameters.getRawParameterValue(paramSymmetry);
    crossfade = *parameters.getRawParameterValue(paramCrossfade);
   // choiceA = *parameters.getRawParameterValue(paramTransferFunctionListA);
    //choiceB = *parameters.getRawParameterValue(paramTransferFunctionListB);
    

}

void WaveshaperAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WaveshaperAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WaveshaperAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals; // ?
    
    // consider mono -> stereo, stereo -> stereo
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    targetGain = *parameters.getRawParameterValue(paramGain);
    auto localTargetGain = targetGain;
    saturation = *parameters.getRawParameterValue(paramSaturation);
    symmetry = *parameters.getRawParameterValue(paramSymmetry);
    crossfade = *parameters.getRawParameterValue(paramCrossfade);
//    choiceA = *parameters.getRawParameterValue(paramTransferFunctionListA);
//    choiceB = *parameters.getRawParameterValue(paramTransferFunctionListB);

    
    if ( localTargetGain != mainGain)
    {
        auto gainRateOfChange = (localTargetGain - mainGain) / buffer.getNumSamples();
        
        for ( int channel = 0; channel < totalNumOutputChannels; ++ channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            auto localMainGain = mainGain;
            
            for( int i = 0;  i < buffer.getNumSamples(); ++i)
            {
                localMainGain += gainRateOfChange;
                double sample = buffer.getSample(channel, i);
                double a = transferFunction_A.transform(TransferFunction::Functions::softClipperCascade3, sample, saturation, symmetry);
                double b = transferFunction_B.transform(TransferFunction::Functions::softClipper, sample, saturation, symmetry);
                channelData[i] = mix(a, b, (double)crossfade) * Decibels::decibelsToGain(localMainGain);
            }
        }
        mainGain = localTargetGain;
    }
    else
    {
        for ( int channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            for ( int i = 0; i < buffer.getNumSamples(); ++i)
            {
               // channelData[sample] = buffer.getSample(channel, sample) * Decibels::decibelsToGain(mainGain);
                double sample = buffer.getSample(channel, i);
                double a = transferFunction_A.transform(TransferFunction::Functions::softClipperCascade3, sample, saturation, symmetry);
                double b = transferFunction_B.transform(TransferFunction::Functions::softClipper, sample, saturation, symmetry);
                channelData[i] = mix(a, b, (double)crossfade) * Decibels::decibelsToGain(mainGain);
             
            }
            
        }
    }
}

//==============================================================================
bool WaveshaperAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* WaveshaperAudioProcessor::createEditor()
{
    return new WaveshaperAudioProcessorEditor(*this);
}

//==============================================================================
void WaveshaperAudioProcessor::getStateInformation (MemoryBlock& destData)
{

}

void WaveshaperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveshaperAudioProcessor();
}

//===========================================================

void WaveshaperAudioProcessor::setGain(double gain)
{
    targetGain = gain;
}

double WaveshaperAudioProcessor::getGain() const
{
    return mainGain;
}
                      

AudioProcessorValueTreeState& WaveshaperAudioProcessor::accessTreeState()
{
    return parameters;
}
