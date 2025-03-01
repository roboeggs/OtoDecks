#include "FileBrowserComponent.h"


FileBrowserComponent::FileBrowserComponent(PlaylistComponent& playlist)
    : playlistComponent(playlist)
{
    thread.startThread();
    addAndMakeVisible(fileTreeComp);

    directoryList.setDirectory(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), true, true);
    //directoryList.addChangeListener(this);

    fileTreeComp.setTitle("Files");
    fileTreeComp.setColour(juce::FileTreeComponent::backgroundColourId, juce::Colours::lightgrey.withAlpha(0.6f));

    fileTreeComp.addListener(this); // Исправлено!

    DBG("FileBrowserComponent initialized");
}

void FileBrowserComponent::resized()
{
    fileTreeComp.setBounds(getLocalBounds());
}

// Реализация методов FileBrowserListener
void FileBrowserComponent::selectionChanged()
{
    DBG("File selection changed");
}

void FileBrowserComponent::fileClicked(const juce::File& file, const juce::MouseEvent& event)
{

}

void FileBrowserComponent::fileDoubleClicked(const juce::File& file)
{

}

void FileBrowserComponent::browserRootChanged(const juce::File& newRoot)
{
    scanAndAddFiles(newRoot);
}
void FileBrowserComponent::scanAndAddFiles(const juce::File& directory)
{
    if (!directory.isDirectory())
        return;

    juce::Array<juce::File> files = directory.findChildFiles(juce::File::findFiles, false, "*.mp3;*.wav;*.flac");

    for (const auto& file : files)
    {
        playlistComponent.addTrack(file.getFullPathName());
    }
}