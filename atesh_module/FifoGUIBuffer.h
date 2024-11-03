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
    AteshFiFo (int BufferSize)
    {
        samplesBuffer.resize (BufferSize);
        samplesBuffer.reserve (BufferSize);
        readAndWriteIndex.setTotalSize (BufferSize);
    }
    
    void setAtomicFlag (bool StartDrawing)
    {
        if (StartDrawing)
        {
            std::fill (samplesBuffer.begin(), samplesBuffer.end(), 0.f);
            readAndWriteIndex.reset();
        }

        readyToWrite.exchange (StartDrawing);
    }
    
    void addToFifo (juce::AudioBuffer<float>& AudioBuffer)
    {
        if (! readyToWrite)
            return;
        
        auto scopedWriter = juce::AbstractFifo::ScopedWrite (readAndWriteIndex, AudioBuffer.getNumSamples());
        
        if (scopedWriter.blockSize1 != 0)
        {
            for (int bufferIndex = 0; bufferIndex < scopedWriter.blockSize1; bufferIndex++)
            {
                auto& currentIndex = samplesBuffer.at (scopedWriter.startIndex1 + bufferIndex);
                currentIndex = AudioBuffer.getSample (0, bufferIndex);
            }
        }
        if (scopedWriter.blockSize2 != 0)
        {
            for (int bufferIndex = 0; bufferIndex < scopedWriter.blockSize2 ; bufferIndex++)
            {
                auto& currentIndex = samplesBuffer.at (scopedWriter.startIndex2 + bufferIndex);
                currentIndex = AudioBuffer.getSample (0, bufferIndex + scopedWriter.blockSize1);
            }
        }
    }
    
    int readFromFiFo (std::vector<float>& ReadBufferRef)
    {
        /* You haven't set the atomic flag from the GUI to tell the DSP to start writing samples*/
        jassert (readyToWrite);
        
        auto scopedReader = juce::AbstractFifo::ScopedRead (readAndWriteIndex, static_cast<int> (ReadBufferRef.size()));
        
        if (scopedReader.blockSize1 != 0)
        {
            for (int bufferIndex = 0; bufferIndex < scopedReader.blockSize1; bufferIndex++)
                ReadBufferRef.at (bufferIndex)  = (samplesBuffer.at (scopedReader.startIndex1 + bufferIndex));
        }
        
        if (scopedReader.blockSize2 != 0)
        {
            for (int bufferIndex = 0; bufferIndex < scopedReader.blockSize2; bufferIndex++)
                ReadBufferRef.at (bufferIndex + scopedReader.blockSize1) = (samplesBuffer.at (scopedReader.blockSize2 + bufferIndex));
        }
        
        return static_cast<int> (scopedReader.blockSize1 + scopedReader.blockSize2);
    }
    
    std::atomic_bool readyToWrite { false };
    
    juce::AbstractFifo readAndWriteIndex { 1024 };
    std::vector<float> samplesBuffer;
};
