
#include "PluginProcessor.h"
#include "PluginEditor.h"

DistortionPluginAudioProcessor::DistortionPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
    // , waveformVisualiserFifo (10000)
    , algorithmParameters (*this)
    , dspAlgorithm (algorithmParameters)
{
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
    dspAlgorithm.prepare (sampleRate, maximumExpectedSamplesPerBlock, getTotalNumOutputChannels());
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

void DistortionPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    dspAlgorithm.process (buffer, midi);
    // waveformVisualiserFifo.addToFifo (buffer);
}

bool DistortionPluginAudioProcessor::hasEditor() const
{
    return false;
}

juce::AudioProcessorEditor* DistortionPluginAudioProcessor::createEditor()
{
    return nullptr;
//    return new DistortionPluginAudioProcessorEditor (*this, algorithmParameters.parameterValueTree, algorithmParameters.parameterIDS);
}

void DistortionPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = algorithmParameters.parameterValueTree.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary (*xml,destData);
}

void DistortionPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{	
	std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data,sizeInBytes));

	if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (algorithmParameters.parameterValueTree.state.getType()))
            algorithmParameters.parameterValueTree.replaceState (juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionPluginAudioProcessor();
}
