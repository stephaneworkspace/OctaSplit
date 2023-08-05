//
// Created by Stéphane on 05.08.23.
//
#pragma once
#include <vector>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "AudioFileProperties.h"

using namespace std;
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

    void textEditorTextChanged(juce::TextEditor &editor) override;

    void fileSelectButtonClicked();

private:
    void timerCallback() override;
    unique_ptr<Drawable> loadSVG(const String &path);

    Label fileLabel;
    Label titleLabel;
    Label channelsLabel;
    Label sampleRateLabel;
    Label durationLabel;
    Label bpmLabel;
    TextEditor bpmEditor;
    Label barLabel;
    TextEditor barEditor;
    TextButton closeButton;
    TextButton splitButton;
    TextButton fileSelectButton;
    TextButton aboutButton;
    unique_ptr<Drawable> svgDrawable1;
    unique_ptr<Drawable> svgDrawable2;
    unique_ptr<Drawable> svgDrawable3;
    float rotationAngle = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};