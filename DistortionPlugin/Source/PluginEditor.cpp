#include "PluginProcessor.h"
#include "PluginEditor.h"


DistortionPluginAudioProcessorEditor::DistortionPluginAudioProcessorEditor (DistortionPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)

{	
	// Links to the plugin Processor
	auto& params = processor.getParameters();

	juce::AudioParameterFloat* GainParameter = (juce::AudioParameterFloat*)params.getUnchecked(0);

	juce::AudioParameterFloat* DriveParameter = (juce::AudioParameterFloat*)params.getUnchecked(1);

	juce::AudioParameterFloat* MixParameter = (juce::AudioParameterFloat*)params.getUnchecked(2);

	juce::AudioParameterInt* comboParameter = (juce::AudioParameterInt*)params.getUnchecked(3);

	// Gain control
	addAndMakeVisible(GainSlider);															// Object made Visible
	GainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);							// Style Set to Rotary
	GainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);	// No textbox
	GainSlider.addListener(this);															// Setup as a listener
	GainSlider.setRange(GainParameter->range.start, GainParameter->range.end,GainParameter->range.interval);				// Setting range the same as parameter in processor
	GainSlider.setValue(*GainParameter);

	// Wet and Dry Mix Control
	MixSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);							// Style Set to Rotary
	addAndMakeVisible(MixSlider);															// Object made Visible
	MixSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	MixSlider.addListener(this);
	MixSlider.setRange(MixParameter->range.start, MixParameter->range.end, MixParameter->range.interval);
	MixSlider.setValue(*MixParameter);

	// Drive Rotary
	DriveSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary); // Style Set to Rotary
	addAndMakeVisible(DriveSlider);								   // Object made Visible
	DriveSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	DriveSlider.addListener(this);
	DriveSlider.setRange(DriveParameter->range.start, DriveParameter->range.end, DriveParameter->range.interval);
	DriveSlider.setValue(*DriveParameter);

	// Combobox for changing distortion types
	addAndMakeVisible(OptionsCombobox);							   // Making ComboBox visible
	
	// Adding Options to dropdown
	OptionsCombobox.addItem("Analogue", 1);
	OptionsCombobox.addItem("Hard Clipping", 2);
	OptionsCombobox.addItem("Sine Fold", 3);
	OptionsCombobox.onChange = [this] {OptionChangeEditor();};		   // Implementing switch function
	OptionsCombobox.addListener(this);
	OptionsCombobox.setSelectedId(*comboParameter);

	// Window Dimensions
	setSize (500, 250);												// Set editor Size
	setResizable(true, true);										// Editor can be resized

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

	switch (*comboParameter)
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
	
	// if the value of the combobox listener is the same as a referenced option box
	if (combobox == &OptionsCombobox)
	{	
		// Link between the combobox and the combobox paramter on the processor
		juce::AudioParameterInt* combo = (juce::AudioParameterInt*)params.getUnchecked(3);
		
		*combo = OptionsCombobox.getSelectedId();

	}

}

// Function using the slidervaluechanged function to link the sliders to the audio parameters being used in the processor.
void  DistortionPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) 
{	
	// creating a link to the processor
	auto& params = processor.getParameters();

	// Dectecting which slider has been changed and linking to the correct audio paramter in the prcoessor
	if (slider == &GainSlider) 
	{

		juce::AudioParameterFloat* GainParameter= (juce::AudioParameterFloat*)params.getUnchecked(0);

			*GainParameter = (float)GainSlider.getValue();


	} 
	else if(slider == &DriveSlider) 
	{
		
		juce::AudioParameterFloat* DriveParameter = (juce::AudioParameterFloat*)params.getUnchecked(1);
		
		*DriveParameter = (float) DriveSlider.getValue();
	
	}
	else if (slider == &MixSlider)
	{

		juce::AudioParameterFloat* MixParameter = (juce::AudioParameterFloat*)params.getUnchecked(2);

		*MixParameter = (float)MixSlider.getValue();

	}
	return;
}
