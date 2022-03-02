#include "PluginProcessor.h"
#include "PluginEditor.h"


DistortionPluginAudioProcessorEditor::DistortionPluginAudioProcessorEditor 
(DistortionPluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState (vts)

{	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Slider Variables*/

	// Gain control
	addAndMakeVisible(GainSlider);															// Object made Visible
	gainAttachment.reset(new SliderAttachment(valueTreeState,"gain",GainSlider));

	// Drive Control
	addAndMakeVisible(DriveSlider);
	driveAttachment.reset(new SliderAttachment(valueTreeState, "drive", DriveSlider));

	// Wet and Dry Mix Control
	addAndMakeVisible(MixSlider);									// Object made Visible
	mixAttachment.reset(new SliderAttachment(valueTreeState, "mix", MixSlider));

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* ComboBox Variables*/

	// Links to the plugin Processor 
	auto& params = processor.getParameters();
	juce::AudioParameterInt* comboParameter = (juce::AudioParameterInt*)params.getUnchecked(3);
	
	// Combobox for changing distortion types
	addAndMakeVisible(OptionsCombobox);							   // Making ComboBox visible
	
	// Adding Options to dropdown
	OptionsCombobox.addItemList(distortionTypes, 1);
	OptionsCombobox.onChange = [this] {OptionChangeEditor(); };
	OptionsCombobox.addListener(this);
	OptionsCombobox.setSelectedId(*comboParameter);

	// Window Dimensions
	setSize (500, 250);												// Set editor Size
	setResizable(true, true);										// Editor can be resized

}

DistortionPluginAudioProcessorEditor::~DistortionPluginAudioProcessorEditor(){}

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
	GainSlider.setBounds(WidthStart,gainHeightStart,200,125); // Setting position and size
	MixSlider.setBounds(WidthStart, 125, 200, 125);			// Setting Postion and size
	DriveSlider.setBounds(comboX, 50, 200, 200);					// Setting position and size
	
	// Combobox
	OptionsCombobox.setBounds(comboX, comboY, comboWidth, comboHeight);				// Setting position and size

}
void DistortionPluginAudioProcessorEditor::OptionChangeEditor()
{

	auto& params = processor.getParameters();
	juce::AudioParameterInt* comboParameter = (juce::AudioParameterInt*)params.getUnchecked(3);

	switch (comboParameter->juce::AudioParameterInt::get())
	{
	case 1:OptionsCombobox.setSelectedId(1); break;	
	
	case 2: OptionsCombobox.setSelectedId(2); break;
	
	case 3: OptionsCombobox.setSelectedId(3); break;
		
		break;
	
	}
}

// ComboBoxchanged function used to link the gui and audioparameter
void  DistortionPluginAudioProcessorEditor::comboBoxChanged(juce::ComboBox* combobox)
{
	
	// Reference to the paramters created in the processor file
	auto& params = processor.getParameters();
	if (combobox == &OptionsCombobox)
	{
		// Link between the combobox and the combobox paramter on the processor
		juce::AudioParameterInt* combo = (juce::AudioParameterInt*)params.getUnchecked(3);

		*combo = OptionsCombobox.getSelectedId();
	}
}

