#pragma once

#include <JuceHeader.h>
#include <functional>

class WaveformDisplay  : public juce::Component,
                         public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** Called when there is a change in the audio thumbnail.*/
	void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    /** Loads an audio file from the given URL and generates its waveform.*/
    void loadURL(juce::URL audioURL);

    /** Set the relative position of the playhead */
	void setPositionRelative(double pos);

    // Callback function to notify the player about the new position
    std::function<void(double)> onPositionChanged;



private:
    // Handles mouse drag events to update the playhead position.
    void mouseDrag(const juce::MouseEvent& e) override;

    juce::AudioThumbnail audioThumb; // Audio thumbnail for generating the waveform
    bool fileLoaded; // Flag indicating whether a file is loaded
    double position; // Current position of the playhead

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};


