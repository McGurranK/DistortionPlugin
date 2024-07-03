#include "PluginProcessor.h"
#include "PluginEditor.h"


DistortionPluginAudioProcessorEditor::DistortionPluginAudioProcessorEditor 
(DistortionPluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), audioProcessor (p)

{
	addAndMakeVisible(GainSlider);
    gainAttachment.reset (new SliderAttachment (vts, "gain",GainSlider));

	addAndMakeVisible(DriveSlider);
    driveAttachment.reset (new SliderAttachment (vts, "drive", DriveSlider));

	addAndMakeVisible(MixSlider);									// Object made Visible
    mixAttachment.reset (new SliderAttachment (vts, "mix", MixSlider));

	setSize (500, 250);												// Set editor Size
	setResizable(true, true);										// Editor can be resized
}

void DistortionPluginAudioProcessorEditor::resized()
{
	int WidthStart = (getWidth() / 3) * 2;
	int gainHeightStart = 0;
	
	int comboX = ((getWidth()/3)*2)/ 5;
	int comboY = getHeight()/ 10;
	int comboWidth = (((getWidth() / 3) * 2)/5)*3;
	int comboHeight = comboWidth/8;

	GainSlider.setBounds(WidthStart,gainHeightStart,200,125); // Setting position and size
	MixSlider.setBounds(WidthStart, 125, 200, 125);			// Setting Postion and size
	DriveSlider.setBounds(comboX, 50, 200, 200);					// Setting position
}
