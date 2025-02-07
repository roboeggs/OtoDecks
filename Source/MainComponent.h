#pragma once

#include <JuceHeader.h>


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
	                   public juce::Button::Listener,
	                   public juce::Slider::Listener   
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(juce::Button *) override;

	void sliderValueChanged(juce::Slider*) override;

    // AudioAppComponent methods
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;


private:
    //==============================================================================
    // Your private member variables go here...

    juce::TextButton playButton{ "PLAY" };
	juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };

    juce::Slider volSlider;

    juce::Random rand;

    double phase;
    double dphase;

    bool playing;

	juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::FileChooser fChooser{ "Select a file..." };

	juce::AudioTransportSource transportSource;

    juce::ResamplingAudioSource resampleSource{juce::&AudioTransportSource, false, 2 }


    void loadURL(juce::URL audioURL);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
