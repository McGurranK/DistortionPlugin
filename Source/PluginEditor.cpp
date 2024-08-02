#include "PluginProcessor.h"
#include "PluginEditor.h"

DistortionPluginAudioProcessorEditor::DistortionPluginAudioProcessorEditor 
(DistortionPluginAudioProcessor& ProcessorRef, juce::AudioProcessorValueTreeState& ValueTreeRef, AteParameterIDs& ParameterIDS)
    : AudioProcessorEditor (&ProcessorRef)
    , gainAttachment (std::make_unique<SliderAttachment> (ValueTreeRef, ParameterIDS.outputGainID, gainSlider))
    , driveAttachment (std::make_unique <SliderAttachment> (ValueTreeRef, ParameterIDS.inputGainID, driveSlider))
    , mixAttachment (std::make_unique <SliderAttachment> (ValueTreeRef, ParameterIDS.mixID, mixSlider))
    , delayTimeAttachment (std::make_unique <SliderAttachment> (ValueTreeRef, ParameterIDS.delayTimeID, delayTimeSlider))
    , delayFeedbackAttachment (std::make_unique <SliderAttachment> (ValueTreeRef, ParameterIDS.delayfeedBackAmountID, delayFeedbackSlider))
    , cuttoffAttachment (std::make_unique <SliderAttachment> (ValueTreeRef, ParameterIDS.filterCuttOffID, filterCuttoff))
    , filterQAttachment (std::make_unique <SliderAttachment> (ValueTreeRef, ParameterIDS.filterQID, filterQ))
    , audioProcessor (ProcessorRef)
{
    const auto initSliders = [&] (juce::Slider& SliderRef, juce::String SliderName)
    {
        addAndMakeVisible (SliderRef);
        SliderRef.setLookAndFeel (&LookAndFeel);
        SliderRef.setName (SliderName);
        SliderRef.setTitle (SliderName);
        SliderRef.setSize (80, 80);
    };
    
    initSliders (gainSlider, "Gain");
    initSliders (mixSlider, "Mix");
    initSliders (driveSlider, "Drive");
    initSliders (delayTimeSlider, "Gain");
    initSliders (delayFeedbackSlider, "Mix");
    initSliders (filterCuttoff, "Drive");
    initSliders (filterQ, "Drive");

    setSize (700, 400);
	setResizable (true, true);
}

void DistortionPluginAudioProcessorEditor::paint (juce::Graphics& GraphicsRef)
{
    GraphicsRef.fillAll (juce::Colours::white);
}

void DistortionPluginAudioProcessorEditor::resized()
{
	gainSlider.setBounds (0, 0, 200,125);
	mixSlider.setBounds (0, 125, 200, 125);
	driveSlider.setBounds (0, 50, 200, 200);
    driveSlider.setBounds (0, 50, 200, 200);
    delayTimeSlider.setBounds (0, 50, 200, 200);
    delayFeedbackSlider.setBounds (0, 50, 200, 200);
    filterCuttoff.setBounds (0, 50, 200, 200);
    filterQ.setBounds (0, 50, 200, 200);
}
