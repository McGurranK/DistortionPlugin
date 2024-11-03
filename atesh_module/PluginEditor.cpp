/*
* Atesh main GUI component
*/

AteshAudioEditor::AteshAudioEditor (AteshAudioProcessor& ProcessorRef)
    : juce::AudioProcessorEditor (&ProcessorRef)
    , graphVisuliser (ProcessorRef.waveformVisualiserFifo)
    , audioProcessor (ProcessorRef)
{
    AteParameterIDs ids; // Created to avoid incorrect use of parameter ID
    
    gainSliderLink.reset (new juce::SliderParameterAttachment (ProcessorRef.getAteshParameter (ids.inputGainID), gainSlider));
    mixSliderLink.reset (new juce::SliderParameterAttachment (ProcessorRef.getAteshParameter (ids.mixID), mixSlider));
    outputSliderLink.reset (new juce::SliderParameterAttachment (ProcessorRef.getAteshParameter (ids.outputGainID), outputGainSlider));
    
    delayTimeSliderLink.reset (new juce::SliderParameterAttachment (ProcessorRef.getAteshParameter (ids.delayTimeID), delayTimeSlider));
    delayFeedbackSliderLink.reset (new juce::SliderParameterAttachment (ProcessorRef.getAteshParameter (ids.delayfeedBackAmountID), delayFeedbackSlider));
    
    filterCuttoffSliderLink.reset (new juce::SliderParameterAttachment (ProcessorRef.getAteshParameter (ids.filterCuttOffID), filterCuttoff));
    filterQSliderLink.reset (new juce::SliderParameterAttachment (ProcessorRef.getAteshParameter (ids.filterQID), filterQ));
    
    setLookAndFeel (&atesLookAndFeel);
    
    addAndMakeVisible (graphVisuliser);

    addAndMakeVisible (gainSlider);
    addAndMakeVisible (mixSlider);
    addAndMakeVisible (outputGainSlider);
    
    addAndMakeVisible (outputGainSlider);
    addAndMakeVisible (delayTimeSlider);
    addAndMakeVisible (delayFeedbackSlider);
    
    addAndMakeVisible (filterCuttoff);
    addAndMakeVisible (filterQ);
    
    setSize (700, 400);
}

AteshAudioEditor::~AteshAudioEditor()
{
    setLookAndFeel (nullptr);
}

void AteshAudioEditor::paint (juce::Graphics& GraphicsRef)
{
    GraphicsRef.fillAll (juce::Colours::black);
}

void AteshAudioEditor::resized()
{
    gainSlider.setBounds (150, 110, 200, 200);
    outputGainSlider.setBounds (0, 250, 150, 150);
    mixSlider.setBounds (0, 0, 150, 150);
    
    delayTimeSlider.setBounds (320, 0, 150, 150);
    delayFeedbackSlider.setBounds (320, 250, 150, 150);
    
    filterCuttoff.setBounds (500, 0, 50, 200);
    filterQ.setBounds (600, 0, 50, 200);
    
    graphVisuliser.setBounds (480, 220, 220, 200);
}
