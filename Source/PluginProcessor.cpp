/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#define _USE_MATH_DEFINES
#include <math.h>

//==============================================================================
DistortionPluginAudioProcessor::DistortionPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
	 ), 
	// Setting up parameter variables
	parameters(*this, nullptr, juce::Identifier("AVTS"),{		
		 
		 // Setting up Gain Parameter (ID, NAME, Min, Max, Default)
		 std::make_unique<juce::AudioParameterFloat>("gain","Gain",0.0f,1.0f,1.0f),
		 
		 // Setting up drive parameter with (ID, NAME, Min, Max, Default)
		 std::make_unique<juce::AudioParameterFloat>("drive","Drive",1.0f,100.0f,1.0f),
		 
		 // Setting up Mix Parameter (ID, NAME, Min, Max, Default)
		 std::make_unique<juce::AudioParameterFloat>("mix","mix",0.0f,1.0f,1.0f)
		 }
		 )
#endif
{	
	// Gets the value set for the parameters gain,drive, and mix using getrawparameters
	mGainParameter = parameters.getRawParameterValue("gain");
	mDriveParameter = parameters.getRawParameterValue("drive");
	mMixParameter = parameters.getRawParameterValue("mix");
	
	// Setting up switch using add parameter (Needs replaced with parameter int)
	addParameter(mSwitchParameter = new juce::AudioParameterInt("switch", "Switch", 1, 3, 2));
	
}

DistortionPluginAudioProcessor::~DistortionPluginAudioProcessor()
{
}

//==============================================================================
const juce::String DistortionPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistortionPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistortionPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistortionPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistortionPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionPluginAudioProcessor::setCurrentProgram (int)
{
	//Do Nothing
}

const juce::String DistortionPluginAudioProcessor::getProgramName (int)
{
	return {};
}

void DistortionPluginAudioProcessor::changeProgramName (int , const juce::String&)
{
	//Do Nothing
}

//==============================================================================
void DistortionPluginAudioProcessor::prepareToPlay (double , int)
{


}

void DistortionPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void DistortionPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
   
	
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	// Interate through the channels
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
		// Channel Data used to write to buffer
		auto *channelData = buffer.getWritePointer(channel);

		//
		for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) 
		{
			// Variable created to store incoming channeldata into a float.
			float cleansig = *channelData;

			// Function used to switch between algorithms
			*channelData = OptionChange(channelData);

			// Mix control between the distorted and clean signal with gain applied at end
			*channelData = ((*channelData**mMixParameter)+(cleansig*(1 - *mMixParameter)))**mGainParameter;
			
			// Interate through each channel
			channelData++;				
		}
	}
}
float DistortionPluginAudioProcessor::OptionChange(float *channelData)
{
	// Switch Statement used to change the type of distortion algorithm
	
	switch (mSwitchParameter->get())			// Take value of Item Selected
	{
	default:									// Print is holder until DSP implemnted
	
	case 1:
		/* Waveshapping algorithm*/
		
		// Multiplying input data 
		*channelData *= *mDriveParameter; 
		
		//using waveshapping on the input data
		*channelData = (float)((2.f / M_PI)* atan(*channelData)); 

		break;				

	case 2: 
		/* HardClipping Algorithm */

		//Multiplying input data
		*channelData *= *mDriveParameter; 
		
		// Using if statement to cut off the top of a waveform
		// if the sample is greater than 0.7 set it to 1.0
		if (*channelData > 0.7) 
		{
			
			*channelData = 1;
		
		}
		// if the sample is less than -0.7 set it to -1.0
		if (*channelData < -0.7) 
		
		{
			
			*channelData = -1;
		}

		break;

	case 3:
		/* Sine Algorithm */

		// using sine on input data
		*channelData = std::sin(*mDriveParameter**channelData); 
		
		// Implement wavetable for greater efficiency
		
		break;

	}
	// return the channeldata as output
	return *channelData;
}

bool DistortionPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistortionPluginAudioProcessor::createEditor()
{
    return new DistortionPluginAudioProcessorEditor (*this,parameters);
}

//==============================================================================
void DistortionPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{	
	auto state = parameters.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml,destData);

}

void DistortionPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{	
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data,sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(parameters.state.getType()))
			parameters.replaceState(juce::ValueTree::fromXml(*xmlState));

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionPluginAudioProcessor();
}
