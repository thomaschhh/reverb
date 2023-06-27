/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ReverbAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    ReverbAudioProcessorEditor (ReverbAudioProcessor&);
    ~ReverbAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
	void sliderValueChanged (juce::Slider* slider) override;
	
	juce::Slider mVolumeSlider;
	juce::Slider mDelaytimeSlider;
	juce::Label mReverbLabel;
	
    ReverbAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbAudioProcessorEditor)
};
