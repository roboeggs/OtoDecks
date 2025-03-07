#pragma once

#include "JuceHeader.h"

// DJAudioPlayer class handles audio playback, including loading, playing, and controlling audio files.
class DJAudioPlayer : public juce::AudioSource
{
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer() override;
    /** Prepares the audio player for playback.*/
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	/** Renders the next audio block to be played.*/
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
	/** Releases the audio player's resources.*/
    void releaseResources() override;

	/** Sets the gain (volume) of the audio player.*/
    void setGain(double newGain);
	/** Sets the speed (playback rate) of the audio player.*/
    void setSpeed(double ratio);
	/** Sets the playback position in seconds.*/
    void setPosition(double posInSecs);
	/** Sets the playback position relative to the total length.*/
	void setPositionRelative(double posInSecs); 

	/* Starts audio playback. */
    void start();
	/* Stops audio playback. */
    void stop();
	/* Checks if the audio player is currently playing. */
    bool isPlaying();
    
    /* Gets the relative position of the playhead. */
    double getPositionRelative();
	/* Gets the playback position in seconds. */
    double getPositionInSeconds();

    void loadURL(const juce::URL& audioURL);

private:
    juce::AudioFormatManager& formatManager; // Reference to the audio format manager
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource; // Source for reading audio data
    juce::AudioTransportSource transportSource; // Source for transporting audio data
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 }; // Source for resampling audio data
};