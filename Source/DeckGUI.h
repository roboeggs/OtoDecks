/*
  ==============================================================================

    DeckGUI.h
    Created: 9 Feb 2025 11:05:53am
    Author:  andfi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "InfiniteRotarySliderComponent.h"

//==============================================================================
/*
*/
class DeckGUI  :    public juce::Component,
	                public juce::Button::Listener, 
	                public juce::Slider::Listener,
	                public juce::FileDragAndDropTarget,
                    public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* player, 
            juce::AudioFormatManager & formatManagerToUse,
            juce::AudioThumbnailCache & cacheToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    /** implement Button::Listener */
    void buttonClicked(juce::Button*) override;
    /** implement Slider::Listener */
    void sliderValueChanged(juce::Slider* slider) override;
    void loadTrack(const juce::URL& audioURL);

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
	void filesDropped(const juce::StringArray& files, int x, int y) override;
    void timerCallback();

private:
    const double speed = 0.8f;
    InfiniteRotarySliderComponent infiniteRotarySlider;

	void playButtonSetColor();

    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    

	DJAudioPlayer* player;

    juce::FileChooser fChooser{ "Select a file..." };

	WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
