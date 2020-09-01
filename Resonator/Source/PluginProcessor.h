/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Resonator.h"

//==============================================================================
/**
*/


namespace paramData
{
    
    class Param
    {
    public:
        Param(juce::String n)
        :paramName{n}
        {
            paramID = paramName + "_ID";
        }
        
        juce::String getID() const {return paramID;}
        juce::String getName() const {return paramName;}
    private:
        juce::String paramName;
        juce::String paramID;
        
    };
    
    enum {freq, fine_tune, q, gain, algo, bypass};
    
    const static std::vector<Param> paramArray {Param{"FREQUENCY"}, Param{"FINETUNE"}, Param{"Q"}, Param{"GAIN"}, Param{"ALGORITHM"}, Param{"BYPASS"}};
}

class ResonatorAudioProcessor  : public juce::AudioProcessor,
public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    ResonatorAudioProcessor();
    ~ResonatorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState& getValueTree();
    
    // list of algorithms
    static juce::StringArray listOfAlgorithms;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    //==============================================================================
    juce::Atomic<double> freqAtom;
    juce::Atomic<double> qAtom;
    juce::Atomic<double> gainAtom;
    juce::Atomic<double> fineTuneAtom;
    juce::Atomic<bool> bypassAtom;
    juce::Atomic<int> algorithmAtom;
    
    double lastGain;
    
    juce::OwnedArray<sfd::Resonator<double>> stereoResonator;
    sfd::FilterParameters<double> resonParameters;
    std::unique_ptr<sfd::OnePole<double>> onePole;
    
    juce::AudioProcessorValueTreeState parameters;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResonatorAudioProcessor)
};
