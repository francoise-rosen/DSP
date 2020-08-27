/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

const juce::String OnePoleAudioProcessor::freqParam = "FREQUENCY_ID";
const juce::String OnePoleAudioProcessor::gainParam = "GAIN_ID";

//==============================================================================
OnePoleAudioProcessor::OnePoleAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
freqAtom{50.0f}, gainAtom{-20.0f}, 
parameters(*this,
           nullptr,
           "LPF_PARAMETERS",
           // Parameter Layout
           {
               std::make_unique<juce::AudioParameterFloat>(freqParam,
                                                           "FREQUENCY",
                                                           juce::NormalisableRange<float>(10.0f, 10000.0f, 0.1f, std::log(0.5f)/std::log(200.0f/9990.0f)),
               freqAtom.get(), "Hz",
               juce::AudioProcessorParameter::genericParameter,
               [](float val, int) {return juce::String(val,2) + "Hz";},
               [](const juce::String& str_value) {return str_value.dropLastCharacters(3).getFloatValue();
               }
               ),
               std::make_unique<juce::AudioParameterFloat>(gainParam,
                                                           "GAIN",
                                                           juce::NormalisableRange<float>(-100.0f, 24.0f, 0.1f, std::log(0.5f)/std::log(94.0f/124.0f)),
                                                           gainAtom.get(), "dB",
                                                           juce::AudioProcessorParameter::genericParameter,
                                                           [](float val, int) {return juce::String(val, 2) + "dB";},
                                                           [](const juce::String& s) {return s.dropLastCharacters(2).getFloatValue();}
                                                           )
               
           }
           )
{
    parameters.addParameterListener(freqParam, this);
    parameters.addParameterListener(gainParam, this);
    
    lastFreq = freqAtom.get();
    lastGain = gainAtom.get();
}

OnePoleAudioProcessor::~OnePoleAudioProcessor()
{
  
}

//==============================================================================


//==============================================================================
void OnePoleAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int i = 0; i < getTotalNumInputChannels(); ++i)
    {
        LPF.push_back(std::make_unique<OnePole<float>>(freqAtom.get(), sampleRate));
    }
    gainSmoother = std::make_unique<OnePole<float>>(10.0f, sampleRate);
}

void OnePoleAudioProcessor::releaseResources()
{
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OnePoleAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void OnePoleAudioProcessor::parameterChanged(const juce::String & paramID
                                             , float newValue)
{
    if (paramID == freqParam)
    {
        freqAtom = newValue;
    }
    
    else if (paramID == gainParam)
    {
        gainAtom = newValue;
    }
}

void OnePoleAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const float freq = freqAtom.get();
    const float gain = juce::Decibels::decibelsToGain(gainAtom.get());

 
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        LPF[channel]->setFrequency(lastFreq);

        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto output = channelData[sample] * gainSmoother->process(lastGain);
            //auto output = channelData[sample] * lastGain;
            //channelData[sample] = output;
            channelData[sample] = LPF[channel]->process(output);
        }
    }
    lastGain = gain;
    lastFreq = freq;
}

//==============================================================================
bool OnePoleAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OnePoleAudioProcessor::createEditor()
{
    return new OnePoleAudioProcessorEditor (*this);
}

//==============================================================================
void OnePoleAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OnePoleAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================

juce::AudioProcessorValueTreeState& OnePoleAudioProcessor::getValueTree()
{
    return parameters;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OnePoleAudioProcessor();
}

const juce::String OnePoleAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OnePoleAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool OnePoleAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool OnePoleAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double OnePoleAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OnePoleAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int OnePoleAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OnePoleAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OnePoleAudioProcessor::getProgramName (int index)
{
    return {};
}

void OnePoleAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}
