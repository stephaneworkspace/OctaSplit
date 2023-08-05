//
// Created by Stéphane on 05.08.23.
//
#pragma once
#include <string>
#include <limits.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "Misc.h"
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

    void filesDropped(const StringArray &files, int x, int y) override;

    bool keyPressed(const KeyPress &key, Component *originatingComponent) override;

    void textEditorTextChanged(TextEditor &editor) override;

    void fileSelectButtonClicked();

    void splitButtonClicked();

    void aboutButtonClicked();

private:
    void timerCallback() override;
    void loadPNG(const String& path);
    unique_ptr<Drawable> loadSVG(const String &path);

    string appName;
    string appVersion;
    Label fileLabel;
    //Label titleLabel;
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
    juce::Image pngImage;
    unique_ptr<Drawable> svgDrawable1;
    unique_ptr<Drawable> svgDrawable2;
    unique_ptr<Drawable> svgDrawable3;
    float rotationAngle = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};