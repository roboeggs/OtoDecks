#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);

	addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
	addAndMakeVisible(loadButton);


    addAndMakeVisible(volSlider);


	playButton.addListener(this);
    stopButton.addListener(this);
	loadButton.addListener(this);
    volSlider.addListener(this);

	volSlider.setRange(0.0, 1.0);

    setAudioChannels(0, 2);
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
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    double rowH = getHeight() / 5;
	playButton.setBounds(0, 0, getWidth(), rowH);
	stopButton.setBounds(0, rowH, getWidth(), rowH);
    volSlider.setBounds(0, rowH * 2 , getWidth(), rowH);
	loadButton.setBounds(0, rowH * 3, getWidth(), rowH);

}

void MainComponent::buttonClicked(juce::Button* button)
{
	if (button == &playButton)
	{
		playing = true;
		juce::Logger::writeToLog("Play button was clicked");
		transportSource.start();

	}
    if (button == &stopButton)
    {
        playing = false;
		juce::Logger::writeToLog("Stop button was clicked");
		transportSource.stop();
    }
	if (button == &loadButton) 
	{
		
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
				loadURL(juce::URL{ chosenFile });
			});

		// source: https://docs.juce.com/master/tutorial_playing_sound_files.html

		//auto chooser = std::make_unique<juce::FileChooser>("Select an audio file", juce::File{}, "*.wav");
		//
		//auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
	
		//chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& chooser)
		//	{
		//		auto file = chooser.getResult();
		//		if (file != juce::File{})
		//		{
		//			auto* reader = formatManager.createReaderFor(file);
		//			if (reader != nullptr)
		//			{
		//				std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
		//				transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		//				readerSource.reset(newSource.release());
		//				transportSource.start();
		//			}
		//		}
		//	});
		// end source
	
	}
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    phase = 0.0;
	dphase = 0.0001;
    // This method is where you should set up any resources you need before playback starts.

	formatManager.registerBasicFormats();
   /* juce::URL audioURL{"file:///C:\\Users\\andfi\\Documents\\Study\\CM2005 Object-Oriented Programming\\Projects\\OtoDecks\\sample.mp3"};

	auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
 
	if (reader != nullptr)
	{
        juce::Logger::writeToLog("File is loaded");
		std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
		transportSource.start();
	}*/

	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);
}

//void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
//{
//    if (!playing)
//    {
//		bufferToFill.clearActiveBufferRegion();
//        return;
//    }
//	auto* leftChan = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
//    auto* rightChan = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
//
//    for (auto i = 0; i < bufferToFill.numSamples; ++i)
//    {
//		//double sample = rand.nextDouble() * 0.25;
//		//double sample = fmod(phase, 0.2);
//        double sample = sin(phase) * 0.1;
//        leftChan[i] = sample;
//		rightChan[i] = sample;
//
//		phase += dphase;
//    }
//
//    // This method is where you should generate the audio data for each block.
//    // For now, we'll just clear the buffer.
//    //bufferToFill.clearActiveBufferRegion();
//}

void MainComponent::releaseResources()
{
    // This method is where you should release any resources you no longer need.
	transportSource.releaseResources();
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    juce::Logger::writeToLog("Slider value changed");
    if (slider == &volSlider)
    {
		transportSource.setGain(slider->getValue());
    }
}

void MainComponent::loadURL(juce::URL audioURL)
{
	
	formatManager.registerBasicFormats();
	std::unique_ptr<juce::InputStream> inputStream(audioURL.createInputStream(false));
	if (inputStream != nullptr)
	{
		auto* reader = formatManager.createReaderFor(std::move(inputStream));
		if (reader != nullptr)
		{
			std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
			transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
			readerSource.reset(newSource.release());
			transportSource.start();
		}
	}
}