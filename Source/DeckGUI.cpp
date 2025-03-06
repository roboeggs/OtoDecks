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
DeckGUI::DeckGUI(DJAudioPlayer* player,
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse)
        :   player(player),
	        waveformDisplay(formatManagerToUse, cacheToUse) 
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(playButton);


    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(infiniteRotarySlider);

	addAndMakeVisible(waveformDisplay);

	volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);


	playButton.addListener(this);

    playButtonSetColor();

	speedSlider.setRange(0, 1);
	volSlider.setRange(0, 100);
	posSlider.setRange(0, 1);
    volSlider.setValue(50);
    speedSlider.setValue(1);

    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(volLabel);

    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(speedLabel);

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


    // Set the onTrackPositionChange callback for the infiniteRotarySlider
    infiniteRotarySlider.onTrackPositionChange = [this, player]() {
        //player->setPositionRelative(newPosition);
        double newPosition = infiniteRotarySlider.getValue() / (180 * speed);
        player->setPosition(newPosition);
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

    // colums

    double colW = getWidth() / 2;
	double rowH = getHeight() / 8;

	// colum 1
	double sliderW = colW / 3;
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);
    volSlider.setBounds(0, rowH * 3, sliderW, rowH * 5);
    speedSlider.setBounds(sliderW * 1, rowH * 3, sliderW, rowH * 5);

    // colum 2
	double buttonW = colW / 4;
    playButton.setBounds(colW + buttonW * 2, rowH * 7, buttonW, rowH);
    infiniteRotarySlider.setBounds(colW, rowH * 3, colW, rowH * 3);

    volLabel.setBounds(0, rowH * 2, sliderW, rowH * 0.5);
    speedLabel.setBounds(sliderW * 1, rowH * 2, sliderW, rowH * 0.5);
}

void DeckGUI::playButtonSetColor() 
{
    playButton.setButtonText(player->isPlaying() ? "Stop" : "Play");
    playButton.setColour(juce::TextButton::buttonColourId, player->isPlaying() ? juce::Colour(0xffed797f) : juce::Colour(0xff00bd16));
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
		loadTrack(juce::URL{ juce::File{files[0]} });
	}
}

void DeckGUI::timerCallback() 
{
    
    double positionRelative = player->getPositionRelative();
    waveformDisplay.setPositionRelative(positionRelative);
	if (player->isPlaying())
	{
	    double positionInSeconds = player->getPositionInSeconds();

        infiniteRotarySlider.setValue(static_cast<int>(positionInSeconds * speed * 180));

        // Calculate the angle based on the relative position
        float angle = static_cast<float>(positionInSeconds * juce::MathConstants<double>::pi * speed);
        infiniteRotarySlider.setAngle(angle);
	}
}

void DeckGUI::loadTrack(const juce::URL& audioURL)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
    playButtonSetColor();
}