/*
* Atesh Plugin Processor
*/

AteshAudioProcessor::AteshAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
    , waveformVisualiserFifo (10000)
    , algorithmParameters (*this)
    , dspAlgorithm (algorithmParameters)
{
}

//==============================================================================
const juce::String AteshAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AteshAudioProcessor::acceptsMidi() const
{
    return false;
}

bool AteshAudioProcessor::producesMidi() const
{
    return false;
}

bool AteshAudioProcessor::isMidiEffect() const
{
    return false;
}

double AteshAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AteshAudioProcessor::getNumPrograms()
{
    return 1;
}

int AteshAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AteshAudioProcessor::setCurrentProgram (int)
{
}

const juce::String AteshAudioProcessor::getProgramName (int)
{
	return {};
}

void AteshAudioProcessor::changeProgramName (int , const juce::String&)
{
}

//==============================================================================
void AteshAudioProcessor::prepareToPlay (double sampleRate, int maximumExpectedSamplesPerBlock)
{
    dspAlgorithm.prepare (sampleRate, maximumExpectedSamplesPerBlock, getTotalNumOutputChannels());
}

void AteshAudioProcessor::releaseResources()
{
}

bool AteshAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void AteshAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    dspAlgorithm.process (buffer, midi);
    waveformVisualiserFifo.addToFifo (buffer);
}

bool AteshAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AteshAudioProcessor::createEditor()
{
    return new AteshAudioEditor (*this);
}

juce::RangedAudioParameter& AteshAudioProcessor::getAteshParameter (const juce::String ParameterID)
{
    return *algorithmParameters.parameterValueTree.getParameter (ParameterID);
}

void AteshAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = algorithmParameters.parameterValueTree.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary (*xml,destData);
}

void AteshAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{	
	std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data,sizeInBytes));

	if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (algorithmParameters.parameterValueTree.state.getType()))
            algorithmParameters.parameterValueTree.replaceState (juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AteshAudioProcessor();
}
