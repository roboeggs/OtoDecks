    /*
      ==============================================================================

        FileBrowserComponent.h
        Created: 1 Mar 2025 11:24:27am
        Author:  andfi

      ==============================================================================
    */


    #pragma once


    #include <JuceHeader.h>
    #include "PlaylistComponent.h"

    class FileBrowserComponent : public juce::Component,
        private juce::FileBrowserListener
    {
    public:
        FileBrowserComponent(PlaylistComponent& playlist);
        void resized() override;

    private:
        void selectionChanged() override;
        void fileClicked(const juce::File& file, const juce::MouseEvent& event) override;
        void fileDoubleClicked(const juce::File& file) override;
        void browserRootChanged(const juce::File& newRoot) override;

        void scanAndAddFiles(const juce::File& directory);

        juce::TimeSliceThread thread{ "File Browser Thread" };
        juce::DirectoryContentsList directoryList{ nullptr, thread };
        juce::FileTreeComponent fileTreeComp{ directoryList };

        PlaylistComponent& playlistComponent; // Ссылка на PlaylistComponent

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileBrowserComponent)
    };