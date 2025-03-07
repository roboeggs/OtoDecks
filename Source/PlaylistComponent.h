#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"

// Structure to hold track information
struct TrackInfo
{
    juce::String path;
    juce::String title;
    double duration = 0.0; // Duration of the track in seconds
};

// PlaylistComponent class handles the playlist, including displaying tracks and handling user interactions.
class PlaylistComponent  :  public juce::Component,
	                        public juce::TableListBoxModel,
                            public juce::Button::Listener
{
public:
    PlaylistComponent(DeckGUI* deckGUI1, DeckGUI* deckGUI2);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** Returns the number of rows in the playlist.*/
    int getNumRows() override;
    /** Paints the background of a row in the playlist.*/
	void paintRowBackground (   juce::Graphics& g, 
                                int rowNumber, 
                                int width, 
                                int height, 
                                bool rowIsSelected) override;
    /** Paints a cell in the playlist.*/
	void paintCell( juce::Graphics& g,
		            int rowNumber,
		            int columnId,
		            int width,
		            int height,
		            bool rowIsSelected) override;
    /** Refreshes the component for a cell in the playlist.*/
    juce::Component* refreshComponentForCell( int rowNumber,
                                        int columnId, 
                                        bool isRowSelected, 
                                        juce::Component* existingComponentToUpdate) override;
    /** Handles button click events.*/
	void buttonClicked(juce::Button* button)  override;
	/** Adds a track to the playlist.*/ 
    void addTrack(const juce::String& trackPath);


private:
    juce::TableListBox tableComponent; // Table component to display the playlist
    std::vector<TrackInfo> tracks; // Vector to hold track information
    DeckGUI* deckGUI1; // Pointer to the first DeckGUI instance
    DeckGUI* deckGUI2; // Pointer to the second DeckGUI instance

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
