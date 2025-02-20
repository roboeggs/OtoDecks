/*
  ==============================================================================

    WaveformDisplay.h
    Created: 12 Feb 2025 9:04:29am
    Author:  andfi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
                         public juce:: ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
	void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void loadURL(juce::URL audioURL);

    /** set the relative position of the playhead */
	void setPositionRelative(double pos);

private:
    void mouseDrag(const juce::MouseEvent& e) override;

	juce::AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
