/*
* Atesh main GUI component
*/

AteshAudioEditor::AteshAudioEditor (AteshAudioProcessor& ProcessorRef)
    : juce::AudioProcessorEditor (&ProcessorRef)
    , header (ProcessorRef.waveformVisualiserFifo)
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
    
    addAndMakeVisible (header);

    addAndMakeVisible (gainSlider);
    addAndMakeVisible (mixSlider);
    addAndMakeVisible (outputGainSlider);
    
    addAndMakeVisible (delayTimeSlider);
    addAndMakeVisible (delayFeedbackSlider);
    
    addAndMakeVisible (filterCuttoff);
    addAndMakeVisible (filterQ);
    
    gainSlider.setTitle ("Input Drive");
    mixSlider.setTitle ("Mix");
    outputGainSlider.setTitle ("Output Gain");
    delayTimeSlider.setTitle ("Delay Time");
    delayFeedbackSlider.setTitle ("Delay Feedback");
    filterCuttoff.setTitle ("Filter Cuttoff");
    filterQ.setTitle ("Filter Q");
    
    for (auto* sliderRef :  { &filterQ, &filterCuttoff })
    {
        sliderRef->setWantsKeyboardFocus (true);
        sliderRef->setAccessible (true);
    }
    
    addAndMakeVisible (pluginName);
    
    pluginName.setText ("Atesh", juce::NotificationType::dontSendNotification);
    pluginName.setFont (juce::Font(juce::FontOptions (20)));
    
    setSize (700, 550);
}

AteshAudioEditor::~AteshAudioEditor()
{
    setLookAndFeel (nullptr);
}

void AteshAudioEditor::paint (juce::Graphics& GraphicsRef)
{
    const auto bounds = getBounds();
    const auto topColour = juce::Colour { 0xff4887a5 };
    const auto bottomColour = juce::Colour { 0x972e2323 };
    
    juce::ColourGradient fillGradient { topColour, juce::Point<float> { static_cast<float> (bounds.getCentreX()), 0.f }
                                      , bottomColour, juce::Point<float> {static_cast<float>(bounds.getCentreX()),static_cast<float>(bounds.getBottom()) }
                                      , false };
    
    GraphicsRef.setGradientFill (fillGradient);
    GraphicsRef.fillAll ();
    
    GraphicsRef.setColour (juce::Colours::white);
    GraphicsRef.drawRect (getBounds());
}

void AteshAudioEditor::resized()
{
    gainSlider.setBounds (137, 210, 200, 200);
    outputGainSlider.setBounds (0, 350, 150, 150);
    mixSlider.setBounds (0, 100, 150, 150);
    
    delayTimeSlider.setBounds (320, 100, 150, 150);
    delayFeedbackSlider.setBounds (320, 350, 150, 150);
    
    filterCuttoff.setBounds (500, 100, 50, 400);
    filterQ.setBounds (600, 100, 50, 400);
    
    header.setBounds (0, 0, getWidth(), 50);
    pluginName.setBounds (0, 0, 50, 50);
}
