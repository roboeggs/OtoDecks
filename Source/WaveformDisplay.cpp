/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 12 Feb 2025 9:04:29am
    Author:  andfi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                    juce::AudioThumbnailCache& cacheToUse) :
                                    audioThumb(1000, formatManagerToUse, cacheToUse),
	                                fileLoaded(false),
                                    position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

	audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{

}

void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    if (fileLoaded) 
    {
		audioThumb.drawChannel(g, 
            getLocalBounds(), 
            0, 
            audioThumb.getTotalLength(),
            0,
            1.0f
        );
        g.setColour(juce::Colours::lightgreen);
        g.fillRect(static_cast<float> (position * getWidth()), 0.0f,
            1.0f, static_cast<float> (getHeight()));
    }
    else
    {
        g.setFont (juce::FontOptions (20.0f));
        g.drawText ("File not loaded...", getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(juce::URL audioURL) {
    juce::Logger::writeToLog("wfd: loadURL");

	audioThumb.clear();

	fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (fileLoaded) {
        juce::Logger::writeToLog("wfd: loaded!");
        repaint();
    }
    else
    {
        juce::Logger::writeToLog("wfd: not loaded!");
    }
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    juce::Logger::writeToLog("wfd: change recevied!");
	repaint();
}

void WaveformDisplay::setPositionRelative(double pos) 
{
    if (pos != position) {
        position = pos;
		repaint();
    }
}

void WaveformDisplay::mouseDrag(const juce::MouseEvent& e)
{
    if (fileLoaded)
    {
        double newPosition = juce::jmax(static_cast<double> (e.x), 0.0) / getWidth();
        newPosition = juce::jlimit(0.0, 1.0, newPosition); // Ensure newPosition is between 0 and 1
        setPositionRelative(newPosition);

        // Notify the player about the new position
        if (onPositionChanged)
        {
            onPositionChanged(newPosition);
        }
    }
}