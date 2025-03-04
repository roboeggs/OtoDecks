/*
  ==============================================================================

    PlaylistComponent.h
    Created: 14 Feb 2025 2:04:20pm
    Author:  andfi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"

//==============================================================================
/*
*/

struct TrackInfo
{
    juce::String path;
    juce::String title;
    double duration = 0.0;
};

class PlaylistComponent  :  public juce::Component,
	                        public juce::TableListBoxModel,
                            public juce::Button::Listener
{
public:
    PlaylistComponent(DeckGUI* deckGUI1, DeckGUI* deckGUI2);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
	void paintRowBackground (   juce::Graphics& g, 
                                int rowNumber, 
                                int width, 
                                int height, 
                                bool rowIsSelected) override;

	void paintCell( juce::Graphics& g,
		            int rowNumber,
		            int columnId,
		            int width,
		            int height,
		            bool rowIsSelected) override;

    juce::Component* refreshComponentForCell( int rowNumber,
                                        int columnId, 
                                        bool isRowSelected, 
                                        juce::Component* existingComponentToUpdate) override;

	void buttonClicked(juce::Button* button)  override;
    void addTrack(const juce::String& trackPath);


private:
    juce::TableListBox tableComponent;
    std::vector<TrackInfo> tracks;
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
