/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

juce::StringArray ResonatorAudioProcessor::listOfAlgorithms {"BiZTRans", "ANaLOgUe",
    "SMMMPLe", "SymmeTRiCAL"};

//==============================================================================
ResonatorAudioProcessor::ResonatorAudioProcessor()
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
freqAtom{1000.0f}, qAtom{1.0f}, gainAtom{-12.0f}, fineTuneAtom{0.0},
bypassAtom{true}, // resonant filter is on by default
algorithmAtom{1}, // default - symmetrical resonant filter

//==============================================================================
// initialise the AudioProcessorValueTreeState object

parameters{*this,
    nullptr,
    juce::Identifier("RESON_PARAMETERS"),
    // ParameterLayout
    createParameterLayout()
}

//==============================================================================
{
    // add parameter listeners
    for (auto p : paramData::paramArray) {
        parameters.addParameterListener(p.getID(), this);
    }
    
    lastGain = gainAtom.get();
    
    resonParameters.frequency = freqAtom.get();
    resonParameters.Q = qAtom.get();
    resonParameters.algorithm = static_cast<sfd::FilterAlgorithm>(algorithmAtom.get());
}

ResonatorAudioProcessor::~ResonatorAudioProcessor()
{
}

//==============================================================================
void ResonatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int i = 0; i < getTotalNumOutputChannels(); ++i)
    {
        stereoResonator.add(new sfd::Resonator<double>(resonParameters, sampleRate));
    }
    
    // 10kHz lopass to smooth out discontinuities of rapid change of the frequency parameters
    onePole = std::make_unique<sfd::OnePole<double>>(10000.0, sampleRate);
}

void ResonatorAudioProcessor::releaseResources()
{
  
}

bool ResonatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // Only mono/stereo and input/output must have same layout
    const auto& mainOutput = layouts.getMainOutputChannelSet();
    const auto& mainInput  = layouts.getMainInputChannelSet();
    
    // input and output layout must either be the same or the input must be disabled altogether
    if (! mainInput.isDisabled() && mainInput != mainOutput)
        return false;
    
    // do not allow disabling the main buses
    if (mainOutput.isDisabled())
        return false;
    
    // only allow stereo and mono
    if (mainOutput.size() > 2)
        return false;
    
    return true;
}


void ResonatorAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == paramData::paramArray[paramData::freq].getID())
    {
        freqAtom = newValue;
    }
    
    else if (parameterID == paramData::paramArray[paramData::fine_tune].getID())
    {
        fineTuneAtom = newValue;
    }
    
    else if (parameterID == paramData::paramArray[paramData::q].getID())
    {
        qAtom = newValue;
    }
    
    else if (parameterID == paramData::paramArray[paramData::gain].getID())
    {
        gainAtom = newValue;
    }
    
    else if (parameterID == paramData::paramArray[paramData::bypass].getID())
    {
        bypassAtom = (bool)newValue;
    }
    
    else if (parameterID == paramData::paramArray[paramData::algo].getID())
    {
        algorithmAtom = (int)newValue;
    }
}

void ResonatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    const float gain = juce::Decibels::decibelsToGain(gainAtom.get());
    const bool bypass = bypassAtom.get();
    
    // smooth out the frequency parameter signam
    resonParameters.frequency = onePole->process(freqAtom.get() + fineTuneAtom.get());
    resonParameters.Q = qAtom.get();
    resonParameters.algorithm = static_cast<sfd::FilterAlgorithm>(algorithmAtom.get());

    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        stereoResonator[channel]->setParameters(resonParameters);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto inputSample = channelData[sample];
            channelData[sample] = stereoResonator[channel]->process(inputSample) * gain;
        }
    }
}

//==============================================================================
bool ResonatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ResonatorAudioProcessor::createEditor()
{
    return new ResonatorAudioProcessorEditor (*this);
}

//==============================================================================
void ResonatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void ResonatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

//==============================================================================
const juce::String ResonatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ResonatorAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ResonatorAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ResonatorAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double ResonatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ResonatorAudioProcessor::getNumPrograms()
{
    return 1;
}

int ResonatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ResonatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ResonatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void ResonatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================

juce::AudioProcessorValueTreeState& ResonatorAudioProcessor::getValueTree()
{
    return parameters;
}

juce::AudioProcessorValueTreeState::ParameterLayout ResonatorAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout parameters;
    
    parameters.add(
    std::make_unique<juce::AudioParameterFloat>(
         paramData::paramArray[paramData::freq].getID(),
         paramData::paramArray[paramData::freq].getName(),
         juce::NormalisableRange<float>(70.0f, 19000.0f, 0.01f,
         std::log(0.5f) / std::log(1000.0f / 18980.0f)),
         freqAtom.get(), "Hz",
         juce::AudioProcessorParameter::genericParameter,
         [](float val, int) {
             return (val < 1000.0f) ? (juce::String(val, 2) + "Hz") :
                 (juce::String(val / 1000.0f, 2) + "kHz");
         },
         [](const juce::String& str_value) {
             return str_value.dropLastCharacters(3).getFloatValue();
         }
        )
                   );
    parameters.add(
    std::make_unique<juce::AudioParameterFloat>(
         paramData::paramArray[paramData::q].getID(),
         paramData::paramArray[paramData::q].getName(),
         (float)sfd::FilterParameters<double>::Q_MIN, (float)sfd::FilterParameters<double>::Q_MAX,
         qAtom.get()
                                                )
                   );
   
   parameters.add(
   std::make_unique<juce::AudioParameterFloat>(
        paramData::paramArray[paramData::gain].getID(),
        paramData::paramArray[paramData::gain].getName(),
        juce::NormalisableRange<float>(-100.0f, 24.0f, 0.01f,
        std::log(0.5f)/std::log(88.0f / 124.0f)),
        gainAtom.get(), "dB",
        juce::AudioProcessorParameter::genericParameter,
        [](float val, int)
        {
            return (val > -100.0f) ? (juce::String(val, 2) + "dB") : "-inf";
        },
        [](const juce::String& str_value) {return str_value.dropLastCharacters(3).getFloatValue();}
                                                )
                  );
    parameters.add(
    std::make_unique<juce::AudioParameterChoice>(
        paramData::paramArray[paramData::algo].getID(),
        paramData::paramArray[paramData::algo].getName(),
        listOfAlgorithms, algorithmAtom.get())
                   );
    
    parameters.add(
    std::make_unique<juce::AudioParameterBool>(
        paramData::paramArray[paramData::bypass].getID(),
        paramData::paramArray[paramData::bypass].getName(),
        bypassAtom.get())
                   );
    parameters.add(
    std::make_unique<juce::AudioParameterFloat>(
        paramData::paramArray[paramData::fine_tune].getID(),
        paramData::paramArray[paramData::fine_tune].getName(),
        -50.0, 50.0,
        fineTuneAtom.get()
                        )
                   );
    
    return parameters;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ResonatorAudioProcessor();
}


