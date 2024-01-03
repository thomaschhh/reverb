/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include <PluginProcessor.h>
#include <PluginEditor.h>

//==============================================================================
ReverbAudioProcessor::ReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), writePositionDelayBuffer (0), startGain(0.1f), endGain(0.7f)
#endif
{
}

ReverbAudioProcessor::~ReverbAudioProcessor()
{
}

//==============================================================================
const juce::String ReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ReverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void ReverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{    
    int delayBufferSize = sampleRate * 2.0;
    delayBuffer.setSize(getTotalNumOutputChannels(), delayBufferSize);
}

void ReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    int bufferSize = buffer.getNumSamples();
    int delayBufferSize = delayBuffer.getNumSamples();
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        fillDelayBuffer(channel, bufferSize, delayBufferSize, channelData);
        readFromDelayBuffer(buffer, delayBuffer, channel, bufferSize, delayBufferSize, channelData);
        fillDelayBuffer(channel, bufferSize, delayBufferSize, channelData);
    }
    
    writePositionDelayBuffer += bufferSize;
    writePositionDelayBuffer %= delayBufferSize;  // ensure to stay in bounds of bufferSize, wrap around in circular buffer
}

void ReverbAudioProcessor::readFromDelayBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, int channel, int bufferSize, int delayBufferSize, float* channelData){
    int readPositionDelayBuffer = writePositionDelayBuffer - getSampleRate();
    
    if (readPositionDelayBuffer < 0){
        readPositionDelayBuffer += delayBufferSize;
    }
    
    if (readPositionDelayBuffer + bufferSize < delayBufferSize) {
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPositionDelayBuffer), bufferSize, endGain, endGain);
    }else{
        int numSamplesToEndOfDelayBuffer = delayBufferSize - readPositionDelayBuffer;
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer (channel, readPositionDelayBuffer), numSamplesToEndOfDelayBuffer, endGain, endGain);
        
        auto numSamplesAtStartOfDelayBuffer = bufferSize - numSamplesToEndOfDelayBuffer;
        buffer.addFromWithRamp(channel, numSamplesToEndOfDelayBuffer, delayBuffer.getReadPointer (channel, 0), numSamplesAtStartOfDelayBuffer, endGain, endGain);
    }
}

void ReverbAudioProcessor::fillDelayBuffer(int channel, int bufferSize, int delayBufferSize, float* channelData){
    // Check to see if main buffer copies to delay buffer without needing to wrap..
    if (delayBufferSize > bufferSize + writePositionDelayBuffer){
        
        // copy main buffer contents to delay buffer
        delayBuffer.copyFromWithRamp(channel, writePositionDelayBuffer, channelData, bufferSize, startGain, startGain);
    }else{
        // copy remaining samples without wrapping around
        int numSamplesToEndOfDelayBuffer = delayBufferSize - writePositionDelayBuffer;
        delayBuffer.copyFromWithRamp(channel, writePositionDelayBuffer, channelData, numSamplesToEndOfDelayBuffer, startGain, startGain);
        
        // start from beginning again
        int numSamplesAtStartOfDelayBuffer = bufferSize - numSamplesToEndOfDelayBuffer;
        delayBuffer.copyFromWithRamp(channel, 0, channelData + numSamplesToEndOfDelayBuffer, numSamplesAtStartOfDelayBuffer, startGain, startGain);
        
    }
}
//==============================================================================
bool ReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReverbAudioProcessor::createEditor()
{
    return new ReverbAudioProcessorEditor (*this);
}

//==============================================================================
void ReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbAudioProcessor();
}
