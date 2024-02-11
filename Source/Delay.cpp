#include <Delay.h>

Delay::Delay(int writePositionDelayBuffer, float startGain, float endGain, int delayBufferSizeSec)
    : writePositionDelayBuffer{writePositionDelayBuffer}, startGain{startGain}, endGain{endGain}, delayDurSec{delayDurSec}
{
}

Delay::~Delay()
{
}

void Delay::fillDelayBuffer(int channel, int bufferSize, float *channelData)
{
    // Check to see if main buffer copies to delay buffer without needing to wrap..
    if (delayBufferSize > bufferSize + writePositionDelayBuffer)
    {

        // copy main buffer contents to delay buffer
        delayBuffer.copyFromWithRamp(channel, writePositionDelayBuffer, channelData, bufferSize, startGain, startGain);
    }
    else
    {
        // copy remaining samples without wrapping around
        int numSamplesToEndOfDelayBuffer = delayBufferSize - writePositionDelayBuffer;
        delayBuffer.copyFromWithRamp(channel, writePositionDelayBuffer, channelData, numSamplesToEndOfDelayBuffer, startGain, startGain);

        // start from beginning again
        int numSamplesAtStartOfDelayBuffer = bufferSize - numSamplesToEndOfDelayBuffer;
        delayBuffer.copyFromWithRamp(channel, 0, channelData + numSamplesToEndOfDelayBuffer, numSamplesAtStartOfDelayBuffer, startGain, startGain);
    }
};

void Delay::readFromDelayBuffer(juce::AudioBuffer<float> &buffer, int channel, int bufferSize, float *channelData)
{
    int readPositionDelayBuffer = writePositionDelayBuffer - delayDurSec;

    if (readPositionDelayBuffer < 0)
    {
        readPositionDelayBuffer += delayBufferSize;
    }

    if (readPositionDelayBuffer + bufferSize < delayBufferSize)
    {
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPositionDelayBuffer), bufferSize, endGain, endGain);
    }
    else
    {
        int numSamplesToEndOfDelayBuffer = delayBufferSize - readPositionDelayBuffer;
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPositionDelayBuffer), numSamplesToEndOfDelayBuffer, endGain, endGain);

        auto numSamplesAtStartOfDelayBuffer = bufferSize - numSamplesToEndOfDelayBuffer;
        buffer.addFromWithRamp(channel, numSamplesToEndOfDelayBuffer, delayBuffer.getReadPointer(channel, 0), numSamplesAtStartOfDelayBuffer, endGain, endGain);
    }
};

void Delay::updateWritePosition(int bufferSize)
{
    writePositionDelayBuffer += bufferSize;
    writePositionDelayBuffer %= delayBufferSize; // ensure to stay in bounds of bufferSize, wrap around in circular buffer
}