///*
//* Waveform waveform visualisation
//*/
//
//#pragma once
//#include "FifoGUIBuffer.h"
//
//class WaveformVisualiser final : public juce::Component
//{
//public:
//    WaveformVisualiser (AteshFiFo& BufferRef) 
//    : waveformBuffer (BufferRef)
//    , vBlackAttachment (this, [&]() { repaint (); })
//    {
//        pollingBuffer.resize (1024);
//        waveFormPath.preallocateSpace (4096);
//        
//        waveformBuffer.setAtomicFlag (true);
//    }
//    
//    ~WaveformVisualiser () override
//    {
//        waveformBuffer.setAtomicFlag (false);
//    }
//    
//    float sampleValueToYPosition (float Value)
//    {
//        auto currentHeight = static_cast<float> (getHeight());
//        return currentHeight - juce::jmap (Value, -1.f, 1.f, 0.f, currentHeight);
//    }
//    
//    float xToXPosition (float BufferPosition, float Width, float BufferSize)
//    {
//        return juce::jmap (BufferPosition, 0.f, BufferSize, 0.f, Width);
//    }
//    
//    
//    void paint (juce::Graphics& GraphicsRef) override
//    {
//        GraphicsRef.fillAll (juce::Colours::purple);
//        
//        std::fill (pollingBuffer.begin(), pollingBuffer.end(), 0.f);
//        
//        const auto numberOfSamplesPolled = waveformBuffer.readFromFiFo (pollingBuffer);
//        
//        if (numberOfSamplesPolled != 0)
//        {
//            waveFormPath.clear();
//            
//            for (int pollingBufferIndex = 0; pollingBufferIndex < numberOfSamplesPolled; pollingBufferIndex++)
//            {
//                const auto currentXPosition = xToXPosition (pollingBufferIndex, getWidth(), numberOfSamplesPolled);
//                const auto currentYPosition = sampleValueToYPosition (pollingBuffer.at (pollingBufferIndex));
//
//                juce::Line<float> newLine { previousPosition, { currentXPosition, currentYPosition} };
//                
//                waveFormPath.addLineSegment (newLine, 4.f);
//                previousPosition.setXY (currentXPosition, currentYPosition);
//            }
//            
//            juce::Line<float> newLine { previousPosition, { static_cast<float>(getWidth()), static_cast<float> (getHeight()) }};
//                
//            juce::Line<float> leftLine { { static_cast<float>(getWidth()), static_cast<float> (getHeight()) }, { 0.f, static_cast<float> (getHeight()) };
//            
//            waveFormPath.addLineSegment (newLine, 4.f);
//            waveFormPath.addLineSegment (leftLine, 4.f);
//            waveFormPath.closeSubPath();
//        }
//        
//        const auto middle = sampleValueToYPosition (0.f);
//        
//        juce::ColourGradient lineGradient (juce::Colours::red, 0.f, middle, juce::Colours::black, static_cast<int> (getWidth()), middle, false);
//
//        
//        GraphicsRef.setGradientFill (lineGradient);
//        GraphicsRef.fillPath (waveFormPath.createPathWithRoundedCorners (10.f));
//    }
//    
//    juce::Point<float> previousPosition { 0.f, 0.f };
//    AteshFiFo& waveformBuffer;
//    std::vector<float> pollingBuffer;
//    juce::Path waveFormPath;
//    
//    
//    juce::VBlankAttachment vBlackAttachment;
//};
