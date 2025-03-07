#include <JuceHeader.h>
#include "DeckGUI.h"

// Constructor: Initializes the DeckGUI with the given player, format manager, and cache.
DeckGUI::DeckGUI(DJAudioPlayer* player,
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse)
        :   player(player),
	        waveformDisplay(formatManagerToUse, cacheToUse) 
{
    // Initialize and add UI components
    addAndMakeVisible(playButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(infiniteRotarySlider);
	addAndMakeVisible(waveformDisplay);

    // Add listeners
	volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
	playButton.addListener(this);

    playButtonSetColor();

    // Set slider ranges and initial values
	speedSlider.setRange(0, 1);
	volSlider.setRange(0, 100);
	posSlider.setRange(0, 1);
    volSlider.setValue(50);
    speedSlider.setValue(1);

    // Set labels
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(volLabel);

    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(speedLabel);

    // Set slider styles
    volSlider.setSliderStyle(juce::Slider::LinearVertical);
    // Text field placement
    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    // Color of the text field
	volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::green);
    volSlider.setNumDecimalPlacesToDisplay(0);
    // Removing the text field outline
    volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    speedSlider.setSliderStyle(juce::Slider::LinearVertical);
    speedSlider.setNumDecimalPlacesToDisplay(3);
    speedSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::green);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    // Removing the text field outline
    speedSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    posSlider.setSliderStyle(juce::Slider::Rotary);

    startTimer(40);

    // Set callbacks for waveform display and infinite rotary slider
    waveformDisplay.onPositionChanged = [this, player](double newPosition) {
            player->setPositionRelative(newPosition);
        };


    // Set the onTrackPositionChange callback for the infiniteRotarySlider
    infiniteRotarySlider.onTrackPositionChange = [this, player]() {
            double newPosition = infiniteRotarySlider.getValue() / (180 * speed);
            player->setPosition(newPosition);
        };
}
    
// Destructor: Cleans up resources.
DeckGUI::~DeckGUI()
{
    stopTimer();
}

// Paints the component
void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
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


// Sets the color of the play button based on the player's state.
void DeckGUI::playButtonSetColor() 
{
    playButton.setButtonText(player->isPlaying() ? "Stop" : "Play");
    playButton.setColour(juce::TextButton::buttonColourId, player->isPlaying() ? juce::Colour(0xffed797f) : juce::Colour(0xff00bd16));
}

// Handles button click events.
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

// Handles slider value change events.
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

// Checks if the component is interested in file drag events.
bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    juce::Logger::writeToLog("DeckGUI::isInterestedInFileDrag");
	return true;
}

// Handles files dropped onto the component.
void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    juce::Logger::writeToLog("DeckGUI::filesDropped");
	if (files.size() == 1)
	{
		loadTrack(juce::URL{ juce::File{files[0]} });
	}
}

// Timer callback for updating the component.
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

// Loads a track from the given URL
void DeckGUI::loadTrack(const juce::URL& audioURL)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
    playButtonSetColor();
}