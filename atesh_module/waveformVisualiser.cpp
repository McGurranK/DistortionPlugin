/*
* WaveformVisualiser.cpp
*/

WaveformVisualiser::WaveformVisualiser (AteshFiFo& BufferRef)
: waveformBuffer (BufferRef)
, pollingAudioBuffer (2, 1024)
, vBlackAttachment (this, [&]() { repaint (); })
{
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
    auto bounds = getLocalBounds().reduced (5);
    
    juce::Path clipBorder;
    clipBorder.addRoundedRectangle (bounds, 5.f);
    
    juce::FloatVectorOperations::fill (pollingAudioBuffer.getWritePointer (0), 0.f, 1024);
    const auto numberOfSamplesPolled = waveformBuffer.readFromFiFo (pollingAudioBuffer);
    
    if (numberOfSamplesPolled != 0)
    {
        waveFormPath.clear();
        waveFormPath.preallocateSpace (4096);
        
        for (int pollingBufferIndex = 0; pollingBufferIndex < numberOfSamplesPolled; pollingBufferIndex++)
        {
            if (pollingBufferIndex == 0)
            {
                const auto currentYPosition = sampleValueToYPosition (pollingAudioBuffer.getSample(0, pollingBufferIndex));
                waveFormPath.startNewSubPath (0.f, static_cast<float> (getWidth() * 0.5f) );
                previousPosition.setXY (0.F, currentYPosition);
            }
            else
            {
                const auto currentXPosition = xToXPosition (pollingBufferIndex, getWidth(), numberOfSamplesPolled);
                const auto currentYPosition = sampleValueToYPosition (pollingAudioBuffer.getSample(0, pollingBufferIndex));
                
                juce::Line<float> newLine { previousPosition, { currentXPosition, currentYPosition} };
                
                waveFormPath.addLineSegment (newLine, 1.f);
                previousPosition.setXY (currentXPosition, currentYPosition);
                
                if (pollingBufferIndex + 1 == numberOfSamplesPolled)
                {
                    juce::Line<float> closingLine { previousPosition,  {static_cast<float>(getWidth()), static_cast<float> (getHeight() * 0.5)  }};
                    waveFormPath.addLineSegment (closingLine, 1.f);
                    previousPosition.setXY (getWidth(), getHeight() * 0.5f);
                }
            }
        }

        waveFormPath.closeSubPath();
    }
    
    {
        juce::Graphics::ScopedSaveState saveState (GraphicsRef);
        GraphicsRef.reduceClipRegion (clipBorder);
        GraphicsRef.fillAll (juce::Colours::black);
        
        GraphicsRef.setColour (juce::Colours::white);
        GraphicsRef.fillPath (waveFormPath);
    }
}
