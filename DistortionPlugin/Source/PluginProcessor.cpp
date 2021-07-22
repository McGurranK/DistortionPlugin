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
                       )
#endif
{	
	addParameter(mGainParameter = new juce::AudioParameterFloat("gain","Gain",juce::NormalisableRange<float>(0.0f,1.0f),1.0f));
	
	addParameter(mDriveParameter = new juce::AudioParameterFloat("drive","Drive",1.0f,100.0f,1.0f));

	addParameter(mMixParameter= new juce::AudioParameterFloat("wet","wet", juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f));

	addParameter(mSwitchParameter = new juce::AudioParameterInt("switch", "Switch", 1, 3, 1));

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
	// In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	// Interate through the channels
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
		// Channel Data used to write to buffer
		auto *channelData = buffer.getWritePointer(channel);

		for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) 
		{
			// Variable created to store incoming channeldata into a float.
			float cleansig = *channelData;
			// Function used to switch between algorithms
			*channelData = OptionChange(channelData);
			// Mix control between the distorted and clean signal.
			*channelData = ((*channelData*mMixParameter->get()) +		
						    (cleansig*(1-mMixParameter->get())))
							*mGainParameter->get();
			
			channelData++;				// Interate through next sample
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
		// Waveshapping algorithm
		*channelData *= mDriveParameter->get(); // Multiplying input data 
		*channelData = (float)((2.f / M_PI)* atan(*channelData)); //using waveshapping on the input data

		break;				

	case 2: 
		//HardClipping
		*channelData *= mDriveParameter->get(); //Multiplying input data
		// Hardclipping distortion
		if (*channelData > 0.7) {
			*channelData = 1;
		}
		if (*channelData < -0.7) {
			*channelData = -1;
		}

		break;

	case 3:
		// Sine Algorithm
		*channelData = std::sin(mDriveParameter->get()**channelData); // using sine on input data
		// Implement wavetable for greater efficiency
		break;

	}
	return *channelData;
}

bool DistortionPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistortionPluginAudioProcessor::createEditor()
{
    return new DistortionPluginAudioProcessorEditor (*this);
}

//==============================================================================
void DistortionPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{	
	// XML file used to save saved state
	std::unique_ptr<juce::XmlElement> xml(new juce::XmlElement("ParamTutorial"));
	xml->setAttribute("gain", (double)*mGainParameter);
	xml->setAttribute("drive", (double)*mDriveParameter);
	xml->setAttribute("mix", (double)*mMixParameter);
	xml->setAttribute("combo",(int)*mSwitchParameter);
	copyXmlToBinary(*xml, destData);
}

void DistortionPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{	
	// XML file information
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState.get() != nullptr)			
		if (xmlState->hasTagName("ParamTutorial"))
			*mGainParameter = (float)xmlState->getDoubleAttribute("gain",1.0f);
			*mDriveParameter = (float)xmlState->getDoubleAttribute("drive",1.0f);
			*mMixParameter = (float)xmlState->getDoubleAttribute("mix",1.0f);
			*mSwitchParameter = (int)xmlState->getDoubleAttribute("combo",1.0f);

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionPluginAudioProcessor();
}
