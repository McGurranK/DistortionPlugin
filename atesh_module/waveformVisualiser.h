/*
* Waveform waveform visualisation
*/

#pragma once

class WaveformVisualiser final : public juce::Component
{
public:
    WaveformVisualiser (AteshFiFo& BufferRef);
    ~WaveformVisualiser () override;
    
    float sampleValueToYPosition (float Value);
    
    float xToXPosition (float BufferPosition, float Width, float BufferSize);

    void paint (juce::Graphics& GraphicsRef) override;
    
    juce::Point<float> previousPosition { 0.f, 0.f };
    AteshFiFo& waveformBuffer;
    std::vector<float> pollingBuffer;
    juce::Path waveFormPath;
    
    
    juce::VBlankAttachment vBlackAttachment;
};
