/*
* WaveformVisualiser.cpp
*/

WaveformVisualiser::WaveformVisualiser (AteshFiFo& BufferRef)
: waveformBuffer (BufferRef)
, vBlackAttachment (this, [&]() { repaint (); })
{
    pollingBuffer.resize (1024);
    waveFormPath.preallocateSpace (4096);
    
    waveformBuffer.setAtomicFlag (true);
}
    
WaveformVisualiser::~WaveformVisualiser ()
{
    waveformBuffer.setAtomicFlag (false);
}
    
float WaveformVisualiser::sampleValueToYPosition (float Value)
{
    auto currentHeight = static_cast<float> (getHeight());
    return currentHeight - juce::jmap (Value, -1.f, 1.f, 0.f, currentHeight);
}
    
float WaveformVisualiser::xToXPosition (float BufferPosition, float Width, float BufferSize)
{
    return juce::jmap (BufferPosition, 0.f, BufferSize, 0.f, Width);
}
    
void WaveformVisualiser::paint (juce::Graphics& GraphicsRef)
{
    // Todo: could use juce audio buffer with a clear instead
    std::fill (pollingBuffer.begin(), pollingBuffer.end(), 0.f);
    
    const auto numberOfSamplesPolled = waveformBuffer.readFromFiFo (pollingBuffer);
    
    if (numberOfSamplesPolled != 0)
    {
        waveFormPath.clear();
 
        for (int pollingBufferIndex = 0; pollingBufferIndex < numberOfSamplesPolled; pollingBufferIndex++)
        {
            if (pollingBufferIndex == 0)
            {
                const auto currentYPosition = sampleValueToYPosition (pollingBuffer.at (pollingBufferIndex));
                
                juce::Line<float> newLine { { 0.f, static_cast<float> (getWidth() * 0.5f) }, { 0.f, currentYPosition} };
                waveFormPath.addLineSegment (newLine, 2.f);
                previousPosition.setXY (0.F, currentYPosition);
            }
            else
            {
                const auto currentXPosition = xToXPosition (pollingBufferIndex, getWidth(), numberOfSamplesPolled);
                const auto currentYPosition = sampleValueToYPosition (pollingBuffer.at (pollingBufferIndex));
                
                juce::Line<float> newLine { previousPosition, { currentXPosition, currentYPosition} };
                
                waveFormPath.addLineSegment (newLine, 1.f);
                previousPosition.setXY (currentXPosition, currentYPosition);
                
                if (pollingBufferIndex + 1 == numberOfSamplesPolled)
                {
                    juce::Line<float> closingLine { previousPosition,  {static_cast<float>(getWidth()), static_cast<float> (getHeight() * 0.5)  }};
                    waveFormPath.addLineSegment (closingLine, 1.f);
                    previousPosition.setXY (getWidth(), getHeight() * 0.5);
                }
            }
        }

        waveFormPath.closeSubPath();
    }
    
    const auto middle = sampleValueToYPosition (0.f);
    
    juce::ColourGradient lineGradient (juce::Colours::red, 0.f, middle, juce::Colours::black, static_cast<int> (getWidth()), middle, false);

    
    GraphicsRef.setGradientFill (lineGradient);
    GraphicsRef.setColour (juce::Colours::white);
    GraphicsRef.fillPath (waveFormPath.createPathWithRoundedCorners (10.f));
}
