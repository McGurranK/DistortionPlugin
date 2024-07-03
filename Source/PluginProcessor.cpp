
#include "PluginProcessor.h"
#include "PluginEditor.h"
#define _USE_MATH_DEFINES
#include <math.h>


DistortionPluginAudioProcessor::DistortionPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
    , parameters(*this, nullptr, juce::Identifier("AVTS"),createAPVTSLayout())
{
    parameters.addParameterListener ("gain", this);
    parameters.addParameterListener ("drive", this);
    parameters.addParameterListener ("mix", this);
    
    waveShaper.functionToUse = [](float SampleValue)
    {
        return tanh (SampleValue);
    };
}

//==============================================================================
const juce::String DistortionPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionPluginAudioProcessor::acceptsMidi() const
{
    return false;
}

bool DistortionPluginAudioProcessor::producesMidi() const
{
    return false;
}

bool DistortionPluginAudioProcessor::isMidiEffect() const
{
    return false;
}

double DistortionPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionPluginAudioProcessor::getNumPrograms()
{
    return 1;
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
}

//==============================================================================
void DistortionPluginAudioProcessor::prepareToPlay (double sampleRate, int maximumExpectedSamplesPerBlock)
{
    juce::dsp::ProcessSpec specification;
    specification.maximumBlockSize = maximumExpectedSamplesPerBlock;
    specification.sampleRate = sampleRate;
    specification.numChannels = getTotalNumInputChannels();
    
    mixControl.prepare (specification);
    inputDriveProcessor.prepare (specification);
    waveShaper.prepare (specification);
    outputGain.prepare (specification);
}

void DistortionPluginAudioProcessor::releaseResources()
{
}

bool DistortionPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void DistortionPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
	
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // Temporary buffer for feedback path here
    
    
    juce::dsp::AudioBlock<float> drySamplesBlock (buffer);
    mixControl.pushDrySamples (drySamplesBlock);
    
    juce::dsp::AudioBlock<float> gainBlock (buffer);
    juce::dsp::ProcessContextReplacing<float> driveContext (gainBlock);
    inputDriveProcessor.process (driveContext);
    waveShaper.process (driveContext);

    // This is the feedback path
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        auto channelData = buffer.getWritePointer (channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            feedbackPath.pushSample (channel, channelData [sample]);
        }
    }
    
    juce::dsp::AudioBlock<float> outputBlock (buffer);
    juce::dsp::ProcessContextReplacing<float> outputGainContext (outputBlock);
    outputGain.process (outputGainContext);
    
    juce::dsp::AudioBlock<float> wetSampleBlock (buffer);
    mixControl.mixWetSamples (wetSampleBlock);
}

bool DistortionPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorValueTreeState::ParameterLayout DistortionPluginAudioProcessor::createAPVTSLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout parameterLayout;
    
    parameterLayout.add (std::make_unique<juce::AudioParameterFloat> ("gain", "Gain", -80.f, 0.0f, 1.0f));
    parameterLayout.add (std::make_unique<juce::AudioParameterFloat> ("drive","Drive", -80.f, 24.0f, 0.0f));
    parameterLayout.add (std::make_unique<juce::AudioParameterFloat> ("mix", "mix", 0.0f, 1.0f, 1.0f));
    
    return parameterLayout;
}

void DistortionPluginAudioProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    if (parameterID == "gain")
        outputGain.setGainDecibels (newValue);
    else if (parameterID == "drive")
        inputDriveProcessor.setGainDecibels (newValue);
    else if (parameterID == "mix")
        mixControl.setWetMixProportion (newValue);
}

juce::AudioProcessorEditor* DistortionPluginAudioProcessor::createEditor()
{
    return new DistortionPluginAudioProcessorEditor (*this, parameters);
}

void DistortionPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	auto state = parameters.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml,destData);

}

void DistortionPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{	
	std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data,sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName (parameters.state.getType()))
			parameters.replaceState (juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionPluginAudioProcessor();
}
