/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 8 Feb 2025 10:28:44am
    Author:  andfi

  ==============================================================================
*/

#include "DJAudioPlayer.h"


DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager)
{

}
DJAudioPlayer::~DJAudioPlayer()
{

}
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	formatManager.registerBasicFormats();
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	//transportSource.getNextAudioBlock(bufferToFill);
	resampleSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources()
{
	transportSource.releaseResources();
	resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(const juce::URL& audioURL)
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
void DJAudioPlayer::setGain(double newGain)
{
	if (newGain < 0 || newGain > 1)
	{
		juce::Logger::writeToLog("DJAudioPlayer::setGain gain should be beetween 0 and 1");
	}
	else
	{
		transportSource.setGain(newGain);
	}

}
void DJAudioPlayer::setSpeed(double ratio)
{
	if (ratio < 0 || ratio > 1)
	{
		juce::Logger::writeToLog("DJAudioPlayer::setSpeed ratio gain should be beetween 0 and 100");
	}
	else
	{
		resampleSource.setResamplingRatio(ratio);
	}
}
void DJAudioPlayer::setPosition(double posInSecs) 
{
	transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double posInSecs)
{
	if (posInSecs < 0 || posInSecs > 1)
	{
		juce::Logger::writeToLog("DJAudioPlayer::setPositionRelative pos gain should be beetween 0 and 1");
	}
	else
	{
		double posInSec = transportSource.getLengthInSeconds() * posInSecs;
		setPosition(posInSec);
	}
}

void DJAudioPlayer::start()
{
	transportSource.start();
}
void DJAudioPlayer::stop()
{
	transportSource.stop();
}

double DJAudioPlayer::getPositionRelative() 
{
	return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}