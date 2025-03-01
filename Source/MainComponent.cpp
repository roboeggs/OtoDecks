#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
    setAudioChannels(0, 2);

	addAndMakeVisible(deckGUI1);
	addAndMakeVisible(deckGUI2);

	addAndMakeVisible(playlistComponent);
	addAndMakeVisible(fileBrowser);

	fileBrowser.setBounds(getLocalBounds());
	formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

 /*   g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);*/
}

void MainComponent::resized()
{
    int width = getWidth();
    int height = getHeight();
    int halfWidth = width / 2;
    int halfHeight = height / 2;

    deckGUI1.setBounds(0, 0, halfWidth, halfHeight);
    deckGUI2.setBounds(halfWidth, 0, halfWidth, halfHeight);

    int thirdWidth = width / 3;
    fileBrowser.setBounds(0, halfHeight, thirdWidth, halfHeight);
    playlistComponent.setBounds(thirdWidth, halfHeight, 2 * thirdWidth, halfHeight);
}


void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // This method is where you should set up any resources you need before playback starts.
	player1.prepareToPlay(samplesPerBlockExpected, sampleRate);

	mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

	mixerSource.addInputSource(&player1, false);
	mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This method is where you should release any resources you no longer need.
	player1.releaseResources();
	player1.releaseResources();
	mixerSource.releaseResources();
}

