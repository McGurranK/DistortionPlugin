#include "PluginProcessor.h"
#include "PluginEditor.h"

DistortionPluginAudioProcessorEditor::DistortionPluginAudioProcessorEditor 
(DistortionPluginAudioProcessor& ProcessorRef, juce::AudioProcessorValueTreeState& ValueTreeRef)
    : AudioProcessorEditor (&ProcessorRef)
    , gainAttachment (std::make_unique<SliderAttachment> (ValueTreeRef, "gain", gainSlider))
    , driveAttachment (std::make_unique <SliderAttachment> (ValueTreeRef, "drive", driveSlider))
    , mixAttachment (std::make_unique <SliderAttachment> (ValueTreeRef, "mix", mixSlider))
    , audioProcessor (ProcessorRef)
{
    const auto initSliders = [&] (juce::Slider& SliderRef, juce::String SliderName)
    {
        addAndMakeVisible (SliderRef);
        SliderRef.setLookAndFeel (&LookAndFeel);
        SliderRef.setName (SliderName);
        SliderRef.setTitle (SliderName);
    };
    
    initSliders (gainSlider, "Gain");
    initSliders (mixSlider, "Mix");
    initSliders (driveSlider, "Drive");

    setSize (500, 250);
	setResizable (true, true);
}

void DistortionPluginAudioProcessorEditor::paint (juce::Graphics& GraphicsRef)
{
    GraphicsRef.fillAll (juce::Colours::white);
}

void DistortionPluginAudioProcessorEditor::resized()
{
	int WidthStart = (getWidth() / 3) * 2;
	int gainHeightStart = 0;
	
	int comboX = ((getWidth()/3)*2)/ 5;

	gainSlider.setBounds(WidthStart,gainHeightStart,200,125); // Setting position and size
	mixSlider.setBounds(WidthStart, 125, 200, 125);			// Setting Postion and size
	driveSlider.setBounds(comboX, 50, 200, 200);					// Setting position
}
