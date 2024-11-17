/*
* AteshFiFo.h
*/

#pragma once

/*
* Fifo for drawing blocks of audio without it dropping out
*/

// Todo: Unit test this code
// Todo: Replace the use of vector with juce::AudioBuffer

class AteshFiFo final
{
public:
    AteshFiFo (int BufferSize, unsigned int NumberOfChannels);
    
    void setAtomicFlag (bool StartDrawing);
    
    void addToFifo (juce::AudioBuffer<float>& AudioBuffer);
    
    int readFromFiFo (juce::AudioBuffer<float>& AudioBuffer);
    
private:
    std::atomic_bool readyToWrite { false };
    
    juce::AbstractFifo readAndWriteIndex { 1024 };
    
    juce::AudioBuffer<float> internalSamplesBuffer;
    
    std::vector<float> samplesBuffer;
};
