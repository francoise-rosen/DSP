/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "OnePole.h"

//==============================================================================
/**
*/
class OnePoleAudioProcessor  : public juce::AudioProcessor,
public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    OnePoleAudioProcessor();
    ~OnePoleAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

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
    
    //==============================================================================
    juce::AudioProcessorValueTreeState& getValueTree();
    //==============================================================================
    void parameterChanged(const juce::String&, float newValue) override;
    static const juce::String freqParam;
    static const juce::String gainParam;

private:
    //==============================================================================
    
    juce::Atomic<float> freqAtom, gainAtom;
    juce::AudioProcessorValueTreeState parameters;
    
    std::unique_ptr<OnePole<float>> gainSmoother;
    std::vector<std::unique_ptr<OnePole<float>>> LPF;
    
    float lastGain, lastFreq;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OnePoleAudioProcessor)
};
