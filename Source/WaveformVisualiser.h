#pragma once

#include <JuceHeader.h>
#include "FifoGUIBuffer.h"

class WaveformVisualiser final : public juce::Component
{
public:
    WaveformVisualiser (AteshFiFo& BufferRef) : waveformBuffer (BufferRef)
    {
        pollingBuffer.reserve (1024);
        pollingBuffer.resize (1024);
        waveFormPath.preallocateSpace (4096);
        
        waveformBuffer.setAtomicFlag (true);
    }
    
    ~WaveformVisualiser () override
    {
        waveformBuffer.setAtomicFlag (false);
    }
    
private:
    
    float sampleValueToYPosition (float Value)
    {
        auto currentHeight = static_cast<float> (Value);
        return currentHeight - juce::jmap (Value, -1.f, 1.f, 0.f, currentHeight);
    }
    
    float xToXPosition (float BufferPosition, float Width, float BufferSize)
    {
        return juce::jmap (BufferPosition, 0.f, BufferSize, 0.f, Width);
    }
    
    
    void paint (juce::Graphics& GraphicsRef) override
    {
        std::fill (pollingBuffer.begin(), pollingBuffer.end(), 0.f);
        waveFormPath.clear();
        
        const auto numberOfSamplesPolled = waveformBuffer.readFromFiFo (pollingBuffer);
        
        if (numberOfSamplesPolled != 0)
        {
            for (int pollingBufferIndex = 0; pollingBufferIndex < numberOfSamplesPolled; pollingBufferIndex++)
            {
                if (pollingBufferIndex == 0)
                {
                    const auto currentYPosition = sampleValueToYPosition (pollingBuffer.at (pollingBufferIndex));
                    waveFormPath.startNewSubPath (0, currentYPosition);
                    previousPosition.setY (currentYPosition);
                    
                }
                else
                {
                    const auto currentXPosition = xToXPosition (pollingBufferIndex, getWidth(), numberOfSamplesPolled);
                    const auto currentYPosition = sampleValueToYPosition (pollingBuffer.at (pollingBufferIndex));

                    juce::Line<float> newLine { previousPosition, { currentXPosition, currentYPosition} };
                    
                    waveFormPath.addLineSegment (newLine, 1.f);
                    previousPosition.setXY (currentXPosition, currentYPosition);
                }
            }
            
            waveFormPath.closeSubPath();
        }
        
        const auto middle = sampleValueToYPosition (0.f);
        
        juce::ColourGradient lineGradient (juce::Colours::red, 0.f, middle, juce::Colours::white, static_cast<int> (getWidth()), middle, false);
        
        GraphicsRef.setGradientFill (lineGradient);
        GraphicsRef.fillPath (waveFormPath);
    }
    
    juce::Point<float> previousPosition { 0.f, 0.f };
    AteshFiFo& waveformBuffer;
    std::vector<float> pollingBuffer;
    juce::Path waveFormPath;
};
