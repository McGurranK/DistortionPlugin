/*
* AteshToolBar.h
*/

#pragma once

/*
* ToolBar for the top of the GUI
*/

class AteshHeader final : public juce::Component
{
public:
    explicit AteshHeader (AteshFiFo& AteshFifoRef) : graphVisualiser (AteshFifoRef)
    {
        addAndMakeVisible (graphVisualiser);
    }
    
private:
    void resized() final
    {
        graphVisualiser.setBounds (getBounds().removeFromRight (200));
    }
    
    void paint (juce::Graphics& GraphicsRef) final
    {
        GraphicsRef.fillAll (juce::Colour { 0x972e2323 });
        
        GraphicsRef.setColour (juce::Colours::white);
        GraphicsRef.drawRect (getBounds());
    }
    
    WaveformVisualiser graphVisualiser;
    juce::Label Logo;
};
