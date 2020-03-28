/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DcblockerAudioProcessor::DcblockerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
                       parameters
                       (*this,
                        nullptr,
                        "PARAMS",
                        {
                            std::make_unique<AudioParameterFloat>(DCBID,
                                                                  DCBNAME,
                                                                  0.0,
                                                                  1.0,
                                                                  0)
                        }
                       )
#endif
{
}

DcblockerAudioProcessor::~DcblockerAudioProcessor()
{
}

//==============================================================================
const String DcblockerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DcblockerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DcblockerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DcblockerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DcblockerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DcblockerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DcblockerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DcblockerAudioProcessor::setCurrentProgram (int index)
{
}

const String DcblockerAudioProcessor::getProgramName (int index)
{
    return {};
}

void DcblockerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DcblockerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto totalNumInputChannels  = getTotalNumInputChannels();
    for (int i = 0; i < totalNumInputChannels; ++i)
    {
        filter.add(new DCblock());
    }
}

void DcblockerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DcblockerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DcblockerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    dcblockPole = *parameters.getRawParameterValue(DCBID);
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        filter[channel]->setPole(dcblockPole);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            auto sample = buffer.getSample(channel, i);
            channelData[i] = filter[channel]->process(sample);
        }
    }
}

//==============================================================================
bool DcblockerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DcblockerAudioProcessor::createEditor()
{
    return new DcblockerAudioProcessorEditor (*this);
}

//==============================================================================
void DcblockerAudioProcessor::getStateInformation (MemoryBlock& destData)
{

}

void DcblockerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

AudioProcessorValueTreeState& DcblockerAudioProcessor::accessTreeState()
{
    return parameters;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DcblockerAudioProcessor();
}


