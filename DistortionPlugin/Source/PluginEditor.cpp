#include "PluginProcessor.h"
#include "PluginEditor.h"


DistortionPluginAudioProcessorEditor::DistortionPluginAudioProcessorEditor (DistortionPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)

{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (300, 300);

	// Gain Roatry slider set for Volume
	Gain.setSliderStyle(juce::Slider::SliderStyle::Rotary); // Style Set to Rotary
	addAndMakeVisible(Gain);								// Object made Visible
															// No Text Box
	Gain.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	Gain.addListener(this);
	Gain.setRange(0.f,1.f,0.01f);
    
	// Wet and Dry Mix Control
	Mix.setSliderStyle(juce::Slider::SliderStyle::Rotary); // Style Set to Rotary
	addAndMakeVisible(Mix);								   // Object made Visible
														   // No Text Box
	Mix.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	Mix.addListener(this);
	Mix.setRange(0.01, 1.0,0.01f);
	
	// Drive Rotary
	Drive.setSliderStyle(juce::Slider::SliderStyle::Rotary); // Style Set to Rotary
	addAndMakeVisible(Drive);								   // Object made Visible
														   // No Text Box
	Drive.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	Drive.addListener(this);
	Drive.setRange(1.0, 100.0f, 1.f);

	// Combobox for changing distortion types
	addAndMakeVisible(Options);							   // Making ComboBox visible
	
	// Adding Options to dropdown
	Options.addItem("Analogue", 1);
	Options.addItem("Hard Clipping", 2);
	Options.addItem("Sine Fold", 3);
	Options.onChange = [this] {OptionChangeEditor();};		   // Implementing switch function
	Options.addListener(this);
	Options.setSelectedId(1);

	// Window Dimensions
	setSize (500, 250);
	setResizable(true, true);

}

DistortionPluginAudioProcessorEditor::~DistortionPluginAudioProcessorEditor()
{

}


void DistortionPluginAudioProcessorEditor::resized()
{	

	// Objects Bounds
	int WidthStart = (getWidth() / 3) * 2;
	int gainHeightStart = 0;


	// Combo Box
	int comboX = ((getWidth()/3)*2)/ 5;
	int comboY = getHeight()/ 10;
	int comboWidth = (((getWidth() / 3) * 2)/5)*3;
	int comboHeight = comboWidth/8;

	// Sliders
	Gain.setBounds(WidthStart,gainHeightStart,200,125); // Setting position and size
	Mix.setBounds(WidthStart, 125, 200, 125);			// Setting Postion and size
	Drive.setBounds(comboX, 50, 200, 200);					// Setting position and size
	
	// Combobox
	Options.setBounds(comboX, comboY, comboWidth, comboHeight);				// Setting position and size

}
void DistortionPluginAudioProcessorEditor::OptionChangeEditor()
{
	switch (Options.getSelectedId())			// Take value of Item Selected
	{
	default:									// Print is holder until DSP implemnted
	case 1:Options.setSelectedId(1); break;					// Print A if 1
	case 2: Options.setSelectedId(2); break;					// Print B if 2
	case 3: Options.setSelectedId(3); break;					// Print C if 3
		break;
	}
}


void  DistortionPluginAudioProcessorEditor::comboBoxChanged(juce::ComboBox*){
	audioProcessor.switchOptions = Options.getSelectedId();
}

void  DistortionPluginAudioProcessorEditor::sliderValueChanged(juce::Slider*) 
{
	audioProcessor.mGainValue = (float) Gain.getValue();
	audioProcessor.mDrive = (float) Drive.getValue();
	audioProcessor.wet = (float) Mix.getValue();

}
