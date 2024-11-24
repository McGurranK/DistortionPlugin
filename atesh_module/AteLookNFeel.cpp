/*
* AteLookAndFeel.cpp
*/

AtesLookAndFeel::AtesLookAndFeel()
{
    setColour (juce::Slider::ColourIds::backgroundColourId, juce::Colour { 0x972e2323 });
    setColour (juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colour { 0xff4887a5 });
    setColour (juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colour::fromFloatRGBA (1.f, 1.f, 1.f, 1.f));
}

void AtesLookAndFeel::drawRotarySlider (juce::Graphics& Graphics, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& SliderRef)
{
    juce::ignoreUnused (rotaryStartAngle, rotaryEndAngle);
    
    const auto backGroundColour = SliderRef.findColour (juce::Slider::ColourIds::backgroundColourId);
    const auto fillColour = SliderRef.findColour (juce::Slider::ColourIds::rotarySliderFillColourId);
    const auto outlineColour = SliderRef.findColour (juce::Slider::ColourIds::rotarySliderOutlineColourId);
    
    auto alphaValue = (SliderRef.hasKeyboardFocus (true) || SliderRef.isMouseOverOrDragging()) ? 1.f : 0.5f;

    const auto indicatorColour = fillColour.overlaidWith (outlineColour.withAlpha (alphaValue));
    
    const auto diameter = width > height ? height : width;
    juce::Rectangle<int> rotaryBounds { x, y, diameter, diameter };
    
    Graphics.setColour (backGroundColour);
    Graphics.fillEllipse (rotaryBounds.toFloat());

    
    Graphics.setColour (indicatorColour);
    Graphics.drawEllipse (rotaryBounds.toFloat().reduced (2.f), 4.f);
    
    const auto indicatorWidth = 15;
    const auto indicatorHeight = 28;
    juce::Rectangle<int> indicatorBounds { static_cast<int>(rotaryBounds.getCentreX() - (indicatorWidth * 0.5))
                                         , rotaryBounds.getBottom() - indicatorHeight - 1
                                         , indicatorWidth, indicatorHeight };
    
    const auto rotationAngle = juce::jmap (sliderPosProportional, 0.f, 1.f, juce::degreesToRadians (15.f), juce::degreesToRadians (345.f));
    
    {
        juce::Path indicatorPath;
        indicatorPath.addRectangle (indicatorBounds.toFloat());
        indicatorPath.applyTransform (juce::AffineTransform::rotation (rotationAngle, rotaryBounds.getCentreX(), rotaryBounds.getCentreY()));
        
        Graphics.setColour (indicatorColour);
        Graphics.fillPath (indicatorPath);
    }
    
    Graphics.setColour (indicatorColour);
    Graphics.drawText (SliderRef.getTitle(), x, y + 10, width, height - 10, juce::Justification::centredTop);
    Graphics.drawText (SliderRef.getTextFromValue (SliderRef.getValue()), x, y, width, height, juce::Justification::centred);
}

void AtesLookAndFeel::drawLinearSlider (juce::Graphics& GraphicsRef, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle, juce::Slider& SliderRef)
{
    juce::ignoreUnused (x, y, height);
    juce::ignoreUnused (minSliderPos, maxSliderPos, sliderPos);
    
    const auto backGroundColour = SliderRef.findColour (juce::Slider::ColourIds::backgroundColourId);
    const auto fillColour = SliderRef.findColour (juce::Slider::ColourIds::rotarySliderFillColourId);
    const auto outlineColour = SliderRef.findColour (juce::Slider::ColourIds::rotarySliderOutlineColourId);
    
    auto alphaValue = (SliderRef.hasKeyboardFocus (true) || SliderRef.isMouseOverOrDragging()) ? 1.f : 0.5f;

    const auto indicatorColour = fillColour.overlaidWith (outlineColour.withAlpha (alphaValue));
    
    
    const auto fillBounds = SliderRef.getLocalBounds().toFloat();
    
    GraphicsRef.setColour (backGroundColour);
    GraphicsRef.fillRoundedRectangle (fillBounds, 10);
    
    const auto normalisedPosition = static_cast<float>( juce::jmap (SliderRef.getValue(), SliderRef.getMinimum(), SliderRef.getMaximum(), 0.0, 1.0));
    const auto currentHeight = normalisedPosition * fillBounds.getHeight();
    const auto currentYPosition = (fillBounds.getHeight() - currentHeight) + fillBounds.getY();
    
    juce::Rectangle<float> currentBounds { 0.f, currentYPosition, static_cast<float>(width), currentHeight };
    
    GraphicsRef.setColour (fillColour);
    GraphicsRef.fillRoundedRectangle (currentBounds.reduced(2), 10);
    
    GraphicsRef.setColour (indicatorColour);
    GraphicsRef.drawRoundedRectangle (fillBounds.reduced (2), 10, 4);
}
