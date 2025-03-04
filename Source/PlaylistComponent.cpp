#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* deckGUI1, DeckGUI* deckGUI2)
    : deckGUI1(deckGUI1), deckGUI2(deckGUI2)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    tableComponent.getHeader().addColumn("Title", 1, 200);
    tableComponent.getHeader().addColumn("Duration", 2, 100);
    tableComponent.getHeader().addColumn("Play L", 3, 100);
    tableComponent.getHeader().addColumn("Play R", 4, 100);
    tableComponent.getHeader().addColumn("Delete", 5, 100);
    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(14.0f));
    g.drawText("PlaylistComponent", getLocalBounds(),
        juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows()
{
    return tracks.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else
    {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowNumber >= tracks.size()) return;

    g.setColour(juce::Colours::white);
    juce::String text;

    if (columnId == 1) text = tracks[rowNumber].title;
    else if (columnId == 2) text = juce::String(tracks[rowNumber].duration, 2) + " sec";

    g.drawText(text, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    juce::Component* existingComponentToUpdate)
{
    if (columnId == 3 || columnId == 4 || columnId == 5)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton(columnId == 3 ? "Play L" : (columnId == 4 ? "Play R" : "Delete"));
            std::string id = std::to_string(rowNumber) + (columnId == 3 ? "_L" : (columnId == 4 ? "_R" : "_D"));
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    std::string id = button->getComponentID().toStdString();
    size_t underscorePos = id.find('_');
    int rowNumber = std::stoi(id.substr(0, underscorePos));
    char action = id[underscorePos + 1];

    if (action == 'L' || action == 'R')
    {
        juce::URL audioURL{ juce::File{ tracks[rowNumber].path } };

        if (action == 'L')
        {
            deckGUI1->loadTrack(audioURL);
            //deckGUI1->player->start();
        }
        else if (action == 'R')
        {
            deckGUI2->loadTrack(audioURL);
            //deckGUI2->player->start();
        }
    }
    else if (action == 'D')
    {
        // Remove the track from the playlist
        tracks.erase(tracks.begin() + rowNumber);
        tableComponent.updateContent();
    }
}

void PlaylistComponent::addTrack(const juce::String& trackPath)
{
    // Check if the track already exists in the playlist
    for (const auto& track : tracks)
    {
        if (track.path == trackPath)
        {
            juce::Logger::writeToLog("Track already exists in the playlist: " + trackPath);
            return; // Track already exists, so do not add it again
        }
    }

    juce::File file(trackPath);
    if (!file.existsAsFile())
        return;

    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

    TrackInfo track;
    track.path = trackPath;
    track.title = file.getFileNameWithoutExtension();

    if (reader)
    {
        track.duration = reader->lengthInSamples / reader->sampleRate;
    }

    // Add the track to the list of tracks
    tracks.push_back(track);
    tableComponent.updateContent();
}
