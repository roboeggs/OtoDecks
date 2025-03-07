#pragma once

#include <JuceHeader.h>
#include "PlaylistComponent.h"

/* FileBrowserComponent class handles the file browsing functionality, allowing users to navigate directories and add
*/
class FileBrowserComponent : public juce::Component,
    private juce::FileBrowserListener
{
public:
    FileBrowserComponent(PlaylistComponent& playlist);
    void resized() override;

private:
    // Called when the file selection changes.
    void selectionChanged() override;
    // Called when a file is clicked.
    void fileClicked(const juce::File& file, const juce::MouseEvent& event) override;
    // Called when a file is double-clicked.
    void fileDoubleClicked(const juce::File& file) override;
    // Called when the root directory of the browser changes.
    void browserRootChanged(const juce::File& newRoot) override;

    // Scans the given directory and adds files to the playlist.
    void scanAndAddFiles(const juce::File& directory);

    juce::TimeSliceThread thread{ "File Browser Thread" }; // Thread for handling file I/O operations
    juce::DirectoryContentsList directoryList{ nullptr, thread }; // List of directory contents
    juce::FileTreeComponent fileTreeComp{ directoryList }; // Component for displaying the file tree

    PlaylistComponent& playlistComponent; // Reference to the PlaylistComponent

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileBrowserComponent)
};