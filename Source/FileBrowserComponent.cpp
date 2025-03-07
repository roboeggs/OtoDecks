#include "FileBrowserComponent.h"


FileBrowserComponent::FileBrowserComponent(PlaylistComponent& playlist)
    : playlistComponent(playlist)
{
    // Start the thread for handling file I/O operations.
    thread.startThread();
    // Add the file tree component to the UI.
    addAndMakeVisible(fileTreeComp);

    // Set the initial directory to the user's music directory.
    directoryList.setDirectory(juce::File::getSpecialLocation(juce::File::userMusicDirectory), true, true);

    // Set the title and background color for the file tree component.
    fileTreeComp.setTitle("Files");
    fileTreeComp.setColour(juce::FileTreeComponent::backgroundColourId, juce::Colours::lightgrey.withAlpha(0.6f));

    // Add this component as a listener to the file tree component.
    fileTreeComp.addListener(this);
}

void FileBrowserComponent::resized()
{
    fileTreeComp.setBounds(getLocalBounds());
}


void FileBrowserComponent::selectionChanged()
{

}

void FileBrowserComponent::fileClicked(const juce::File& file, const juce::MouseEvent& event)
{
    // If the clicked file is a directory, scan and add its files to the playlist.
    if (file.isDirectory())
    {
        scanAndAddFiles(file);
    }
}

void FileBrowserComponent::fileDoubleClicked(const juce::File& file)
{

}
void FileBrowserComponent::browserRootChanged(const juce::File& newRoot)
{
    juce::Logger::writeToLog("browserRootChanged called: " + newRoot.getFullPathName());
    scanAndAddFiles(newRoot);
}

void FileBrowserComponent::scanAndAddFiles(const juce::File& directory)
{
    if (!directory.isDirectory())
        return;

    // Find all audio files in the directory.
    juce::Array<juce::File> files = directory.findChildFiles(juce::File::findFiles, false, "*.mp3;*.wav;*.flac");

    // Add each found file to the playlist.
    for (const auto& file : files)
    {
        playlistComponent.addTrack(file.getFullPathName());
    }
}