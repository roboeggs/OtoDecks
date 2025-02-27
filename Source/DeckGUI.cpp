﻿/*
  ==============================================================================

    DeckGUI.cpp
    Created: 9 Feb 2025 11:05:53am
    Author:  andfi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* player,
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse)
        :   player(player),
	        waveformDisplay(formatManagerToUse, cacheToUse) 
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);


    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    //addAndMakeVisible(posSlider);
    addAndMakeVisible(infiniteRotarySlider);

	addAndMakeVisible(waveformDisplay);

	volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);


	playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    playButtonSetColor();

	speedSlider.setRange(0, 1);
	volSlider.setRange(0, 100);
	posSlider.setRange(0, 1);
    volSlider.setValue(50);
    speedSlider.setValue(1);

    volSlider.setSliderStyle(juce::Slider::LinearVertical);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20); // Размещение текстового поля
	volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::green); // Цвет текстового поля
    volSlider.setNumDecimalPlacesToDisplay(0);
    volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack); // Убираем обводку текстового поля


    speedSlider.setSliderStyle(juce::Slider::LinearVertical);
    speedSlider.setNumDecimalPlacesToDisplay(3);
    speedSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::green);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    speedSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack); // Убираем обводку текстового поля

    posSlider.setSliderStyle(juce::Slider::Rotary);
    //posSlider.setRotaryParameters(juce::Slider::RotaryParameters::endAngleRadians);

    startTimer(40);

    // Устанавливаем колбэк для waveformDisplay
    waveformDisplay.onPositionChanged = [this, player](double newPosition) {
        player->setPositionRelative(newPosition);
        };
    // Set the onValueChange callback for the infiniteRotarySlider
    infiniteRotarySlider.onValueChange = [this, player]() {
        double newPosition = infiniteRotarySlider.getValue() / 100.0;
        player->setPositionRelative(newPosition);
        };

    // Set the onTrackPositionChange callback for the infiniteRotarySlider
    infiniteRotarySlider.onTrackPositionChange = [this, player](double newPosition) {
        player->setPositionRelative(newPosition);
        };
}
    
DeckGUI::~DeckGUI()
{
    stopTimer();
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
 
    /** Draw infiniteRotarySlider's value */
    g.drawText((juce::String)infiniteRotarySlider.getAdjustedValue(),
        0, 0, getWidth(), getHeight(),
        juce::Justification::right);
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    /*
    double rowH = getHeight() / 10;
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    volSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH * 2);
	waveformDisplay.setBounds(0, rowH * 7, getWidth(), rowH * 2);
    loadButton.setBounds(0, rowH * 9, getWidth(), rowH);
    */

    // colums

    double colW = getWidth() / 2;
	double rowH = getHeight() / 8;

	// colum 1
	double sliderW = colW / 2;
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);
    volSlider.setBounds(0, rowH * 2, sliderW, rowH * 5);
    speedSlider.setBounds(sliderW * 1, rowH * 2, sliderW, rowH * 5);

    // colum 2
    //posSlider.setBounds(colW, rowH * 2, colW, rowH * 3);
    playButton.setBounds(colW, rowH * 5, colW / 2, rowH);
    stopButton.setBounds(colW + colW / 2, rowH * 5, colW / 2, rowH);
    loadButton.setBounds(colW, rowH * 6, colW, rowH);

    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    /*auto area = getLocalBounds();
    area.reduce(10, 10);*/
    infiniteRotarySlider.setBounds(colW, rowH * 2, colW, rowH * 3);
}

void DeckGUI::playButtonSetColor() 
{
    playButton.setButtonText(player->isPlaying() ? "Stop" : "Play");
    playButton.setColour(juce::TextButton::buttonColourId, player->isPlaying() ? juce::Colour(0xffed797f) : juce::Colour(0xff79ed7f));
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
    	juce::Logger::writeToLog("Play button was clicked");
        if (player->isPlaying()) {
			player->stop();
        }
        else {
    	    player->start();
        }
        playButtonSetColor();
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
                waveformDisplay.loadURL(juce::URL{ chosenFile });

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

void DeckGUI::timerCallback() 
{
    //juce::Logger::writeToLog("DeckGUI::timerCallback");
	if (player->isPlaying())
	{
        double speed = 0.4f;
        double positionRelative = player->getPositionRelative();
	    double positionInSeconds = player->getPositionInSeconds();

        waveformDisplay.setPositionRelative(positionRelative);
        infiniteRotarySlider.setValue(static_cast<int>(positionInSeconds * speed * 180));

        // Calculate the angle based on the relative position
        float angle = static_cast<float>(positionInSeconds * juce::MathConstants<double>::pi * speed);
        infiniteRotarySlider.setAngle(angle);
	}
}