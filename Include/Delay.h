#pragma once

#include <JuceHeader.h>

class Delay 
{
public:
    Delay(int writePositionDelayBuffer = 0, float startGain = 0.1f, float endGain = 0.7f, int delayDurSec = 2);
    ~Delay();

    void fillDelayBuffer(int channel, int bufferSize, float* channelData);
    void readFromDelayBuffer(juce::AudioBuffer<float>& buffer, int channel, int bufferSize, float* channelData);
    void updateWritePosition(int bufferSize);
    
    juce::AudioBuffer<float> delayBuffer;
    int delayDurSec;
    int delayBufferSize;
private:
    int writePositionDelayBuffer;
    float startGain;
    float endGain;
};