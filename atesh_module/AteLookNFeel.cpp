/*
* AteLookAndFeel.cpp
*/

AtesLookAndFeel::AtesLookAndFeel()
{
    setColour (juce::Slider::ColourIds::backgroundColourId, juce::Colour::fromFloatRGBA (0.f, 0.f, 0.2f, 0.6f));
    setColour (juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colour::fromFloatRGBA (1.f, 1.f, 1.f, 1.f));
}

void AtesLookAndFeel::drawRotarySlider (juce::Graphics& Graphics, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& SliderRef)
{
    const auto diameter = width > height ? height : width;
    juce::Rectangle<int> rotaryBounds { x, y, diameter, diameter };
    
    Graphics.setColour (SliderRef.findColour (juce::Slider::ColourIds::backgroundColourId));
    Graphics.fillEllipse (rotaryBounds.toFloat());
    
    Graphics.setColour (juce::Colours::white);
    Graphics.drawEllipse (rotaryBounds.toFloat().reduced (2.f), 4.f);
    
    const auto indicatorWidth = 15;
    const auto indicatorHeight = 30;
    juce::Rectangle<int> indicatorBounds { static_cast<int>(rotaryBounds.getCentreX() - (indicatorWidth * 0.5))
                                         , rotaryBounds.getBottom() - indicatorHeight
                                         , indicatorWidth, indicatorHeight };
    
    const auto rotationAngle = juce::jmap (sliderPosProportional, 0.f, 1.f, juce::degreesToRadians (15.f), juce::degreesToRadians (345.f));
    
    {
        juce::Path indicatorPath;
        indicatorPath.addRectangle (indicatorBounds.toFloat());
        indicatorPath.applyTransform (juce::AffineTransform::rotation (rotationAngle, rotaryBounds.getCentreX(), rotaryBounds.getCentreY()));
        
        Graphics.setColour (juce::Colours::white);
        Graphics.fillPath (indicatorPath);
    }
    
    Graphics.setColour (juce::Colours::red);
    Graphics.drawText (SliderRef.getName(), x, y, width, height, juce::Justification::centredTop);
    Graphics.drawText (SliderRef.getTextFromValue (SliderRef.getValue()), x, y, width, height, juce::Justification::centredBottom);
}
