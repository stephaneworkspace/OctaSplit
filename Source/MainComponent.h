//
// Created by Stéphane on 05.08.23.
//
#pragma once
#include <vector>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "AudioFileProperties.h"

using namespace juce;

class MainComponent: public Component,
                     public KeyListener,
                     public TextEditor::Listener,
                     public DragAndDropContainer,
                     public FileDragAndDropTarget,
                     private Timer {
public:
    MainComponent();

    ~MainComponent() override;

    void paint(Graphics &g) override;

    void update();

    void resized() override;

    bool isInterestedInFileDrag(const StringArray &files) override;

    void filesDropped(const juce::StringArray &files, int x, int y) override;

    bool keyPressed(const juce::KeyPress &key, juce::Component *originatingComponent) override;

    void textEditorTextChanged(juce::TextEditor &editor);

    void fileSelectButtonClicked();

private:
    void timerCallback() override;

    juce::Label fileLabel;
    juce::Label titleLabel;
    juce::Label channelsLabel;
    juce::Label sampleRateLabel;
    juce::Label durationLabel;
    juce::Label bpmLabel;
    juce::TextEditor bpmEditor;
    juce::Label barLabel;
    juce::TextEditor barEditor;
    juce::TextButton closeButton;
    juce::TextButton splitButton;
    juce::TextButton fileSelectButton;
    std::unique_ptr<juce::Drawable> svgDrawable1;
    std::unique_ptr<juce::Drawable> svgDrawable2;
    std::unique_ptr<juce::Drawable> svgDrawable3;
    float rotationAngle = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};