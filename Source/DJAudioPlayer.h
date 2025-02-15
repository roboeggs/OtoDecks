/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 8 Feb 2025 10:28:44am
    Author:  andfi

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public juce::AudioSource
{
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer() override;
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    
    void setGain(double newGain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
	void setPositionRelative(double posInSecs); 

    void start();
    void stop();
    
    /* get the relative position of the playhead */
    double getPositionRelative();

    void loadURL(const juce::URL& audioURL);

private: 
    juce::AudioFormatManager& formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
	juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};