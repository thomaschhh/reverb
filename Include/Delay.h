#pragma once

#include <JuceHeader.h>

class Delay 
{
public:
    Delay() : writePositionDelayBuffer {0}, startGain {0.1f}, endGain {0.7f}, delayBufferSize {2}{};
    ~Delay();

    void fillDelayBuffer(int channel, int bufferSize, int delayBufferSize, float* channelData);
    void readFromDelayBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, int channel, int bufferSize, int delayBufferSize, float* channelData);
    
    juce::AudioBuffer<float> delayBuffer;
    int delayBufferSize;
private:
    int writePositionDelayBuffer;
    float startGain;
    float endGain;
};