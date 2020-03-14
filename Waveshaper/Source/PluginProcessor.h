/*
  ==============================================================================


  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "TransferFunction.h"
#define GAIN_NAME "Gain"
#define SATURATION_NAME "Saturation"
#define SYMMETRY_NAME "Symmetry"
#define FUNCTIONS_A_NAME "Functions_A"
#define FUNCTIONS_B_NAME "Functions_B"
#define CROSSFADE_NAME "Crossfade"

//==============================================================================
/**
*/
class WaveshaperAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    WaveshaperAudioProcessor();
    ~WaveshaperAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //===============================================================================
    void setGain(double);
    double getGain() const;
    AudioProcessorValueTreeState& accessTreeState();
    
   
    
    static String paramGain;
    static String paramSaturation;
    static String paramSymmetry;
    static String paramCrossfade;
    static String paramTransferFunctionListA;
    static String paramTransferFunctionListB;
    

private:
    double mainGain;
    double targetGain;
    float saturation;
    float symmetry;
    AudioProcessorValueTreeState parameters;
    TransferFunction transferFunction; // so far just one
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveshaperAudioProcessor)
};
