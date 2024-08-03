/*
* Atesh main GUI component
*/

AteshAudioEditor::AteshAudioEditor (AteshAudioProcessor& ProcessorRef)
    : juce::AudioProcessorEditor (&ProcessorRef)
    , audioProcessor (ProcessorRef)
{
    setSize (700, 400);
	setResizable (true, true);
}

void AteshAudioEditor::paint (juce::Graphics& GraphicsRef)
{
    GraphicsRef.fillAll (juce::Colours::white);
}

void AteshAudioEditor::resized()
{
}
