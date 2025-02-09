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

//==============================================================================
/*
*/
class DeckGUI  :    public juce::Component,
	                public juce::Button::Listener,
	                public juce::Slider::Listener,
	                public juce::FileDragAndDropTarget
{
public:
    DeckGUI(DJAudioPlayer* player);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    /** implement Button::Listener */
    void buttonClicked(juce::Button*) override;
    /** implement Slider::Listener */
    void sliderValueChanged(juce::Slider* slider) override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
	void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

	DJAudioPlayer* player;

    juce::FileChooser fChooser{ "Select a file..." };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
