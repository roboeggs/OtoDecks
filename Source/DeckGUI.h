#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "InfiniteRotarySliderComponent.h"

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
    /** Checks if the component is interested in file drag events. */
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    /** Handles files dropped onto the component. */
	void filesDropped(const juce::StringArray& files, int x, int y) override;
    /** Timer callback for updating the component. */
    void timerCallback();

private:
    // Wheel rotation speed
    const double speed = 0.8f;
    
    InfiniteRotarySliderComponent infiniteRotarySlider;

    /** Sets the color of the play button based on the player's state. */
	void playButtonSetColor();

    // UI components
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    juce::Label volLabel;
    juce::Label speedLabel;

	DJAudioPlayer* player;
    juce::FileChooser fChooser{ "Select a file..." };
	WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
