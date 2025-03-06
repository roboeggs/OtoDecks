#include "FileBrowserComponent.h"


FileBrowserComponent::FileBrowserComponent(PlaylistComponent& playlist)
    : playlistComponent(playlist)
{
    thread.startThread();
    addAndMakeVisible(fileTreeComp);

    directoryList.setDirectory(juce::File::getSpecialLocation(juce::File::userMusicDirectory), true, true);
;

    fileTreeComp.setTitle("Files");
    fileTreeComp.setColour(juce::FileTreeComponent::backgroundColourId, juce::Colours::lightgrey.withAlpha(0.6f));

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

    juce::Array<juce::File> files = directory.findChildFiles(juce::File::findFiles, false, "*.mp3;*.wav;*.flac");

    for (const auto& file : files)
    {
        playlistComponent.addTrack(file.getFullPathName());
    }
}