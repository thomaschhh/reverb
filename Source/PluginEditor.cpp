/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverbAudioProcessorEditor::ReverbAudioProcessorEditor (ReverbAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p)
{
	mReverbLabel.setColour(juce::Label::textColourId, juce::Colours::black);
	mReverbLabel.setText("REVERB", juce::NotificationType::dontSendNotification);
	mReverbLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&mReverbLabel);
	
	mVolumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	mVolumeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, false, 90, 30);
	mVolumeSlider.setSkewFactorFromMidPoint(7);
	mVolumeSlider.setRange(-96, 0, 0.1);
	mVolumeSlider.setValue(-12);
	mVolumeSlider.setNumDecimalPlacesToDisplay(1);
	mVolumeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
	mVolumeSlider.setTextValueSuffix(" dBFS");
	mVolumeSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
	mVolumeSlider.addListener(this);
	addAndMakeVisible(&mVolumeSlider);
	
	mDelaytimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	mDelaytimeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, false, 80, 30);
	mDelaytimeSlider.setSkewFactorFromMidPoint(3);
	mDelaytimeSlider.setRange(50, 2000, 1);
	mDelaytimeSlider.setValue(500);
	mDelaytimeSlider.setNumDecimalPlacesToDisplay(0);
	mDelaytimeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
	mDelaytimeSlider.setTextValueSuffix(" ms");
	mDelaytimeSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
	mDelaytimeSlider.addListener(this);
	addAndMakeVisible(&mDelaytimeSlider);
	
    setSize (400, 300);
}

ReverbAudioProcessorEditor::~ReverbAudioProcessorEditor()
{
}

void ReverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::white);
}

void ReverbAudioProcessorEditor::resized()
{
	juce::Rectangle<int> bounds = getLocalBounds();
	
	juce::FlexBox volumeFlexbox;
	volumeFlexbox.flexDirection = juce::FlexBox::Direction::row;
	volumeFlexbox.items.add(juce::FlexItem(mVolumeSlider).withFlex(1));
	
	juce::FlexBox delayFlexbox;
	delayFlexbox.flexDirection = juce::FlexBox::Direction::row;
	delayFlexbox.items.add(juce::FlexItem(mDelaytimeSlider).withFlex(1));
	
	juce::FlexBox groupVolumeAndDelayFlexbox;
	groupVolumeAndDelayFlexbox.flexDirection = juce::FlexBox::Direction::row;
	groupVolumeAndDelayFlexbox.items.add(juce::FlexItem(volumeFlexbox).withFlex(1));
	groupVolumeAndDelayFlexbox.items.add(juce::FlexItem(delayFlexbox).withFlex(1));
	
	juce::FlexBox reverbFlexbox;
	reverbFlexbox.flexDirection = juce::FlexBox::Direction::column;
	reverbFlexbox.items.add(juce::FlexItem(mReverbLabel).withFlex(2));
	reverbFlexbox.items.add(juce::FlexItem(groupVolumeAndDelayFlexbox).withFlex(5));
	
	reverbFlexbox.performLayout(bounds);
}

void ReverbAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
	
}
