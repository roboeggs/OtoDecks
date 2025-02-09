/*
  ==============================================================================

    DeckGUI.cpp
    Created: 9 Feb 2025 11:05:53am
    Author:  andfi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* player) : player(player)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);


    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

	volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);


	playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

	volSlider.setRange(0, 1);
	speedSlider.setRange(0, 100);
	posSlider.setRange(0, 1);
}

DeckGUI::~DeckGUI()
{
}

void DeckGUI::paint (juce::Graphics& g)
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
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("DeckGUI", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    double rowH = getHeight() / 6;
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    volSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH);
    loadButton.setBounds(0, rowH * 5, getWidth(), rowH);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
    	juce::Logger::writeToLog("Play button was clicked");
    	player->start();

    }
    if (button == &stopButton)
    {
    	juce::Logger::writeToLog("Stop button was clicked");
    	player->stop();
    }
    if (button == &loadButton) 
    {
    	// source: https://docs.juce.com/master/tutorial_playing_sound_files.html
    	// this does work in 6.1 but the syntax is a little funky
    	// https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
    	// - configure the dialogue
    	auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
    	// - launch out of the main thread
    	// - note how we use a lambda function which you've probably
    	// not seen before. Please do not worry too much about that. 
    	fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
    		{
    			juce::File chosenFile = chooser.getResult();
    			player->loadURL(juce::URL{ chosenFile });
    		});
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
        if (slider == &volSlider)
        {
            player->setGain(volSlider.getValue());
        }
        if (slider == &speedSlider)
        {
            player->setSpeed(speedSlider.getValue());
        }
        if (slider == &posSlider)
        {
            player->setPositionRelative(posSlider.getValue());
        }

}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    juce::Logger::writeToLog("DeckGUI::isInterestedInFileDrag");
	return true;
}
void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    juce::Logger::writeToLog("DeckGUI::filesDropped");
	if (files.size() == 1)
	{
		player->loadURL(juce::URL{ juce::File{files[0]} });
	}
}