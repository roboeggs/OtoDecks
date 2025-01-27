#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);

	addAndMakeVisible(playButton);
    addAndMakeVisible(volSlider);

	playButton.addListener(this);
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

    g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    double rowH = getHeight() / 5;
	playButton.setBounds(0, 0, getWidth(), rowH);
    volSlider.setBounds(0, rowH, getWidth(), rowH);

}

void MainComponent::buttonClicked(juce::Button* button)
{
    std::cout << "Button was clicked" << std::endl;
	if (button == &playButton)
	{
		juce::Logger::writeToLog("Play button clicked");
	}
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // This method is where you should set up any resources you need before playback starts.
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // This method is where you should generate the audio data for each block.
    // For now, we'll just clear the buffer.
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This method is where you should release any resources you no longer need.
}