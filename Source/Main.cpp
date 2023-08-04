#include <sndfile.hh>
#include <vector>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "AudioFileProperties.h"

class MainComponent : public juce::Component, public juce::DragAndDropContainer, public juce::FileDragAndDropTarget {
public:
    MainComponent()
            : fileLabel("", "No file loaded..."),
              closeButton("Close"),
              bpmEditor(),
              barEditor()
    {
        setSize(800, 600);
        setWantsKeyboardFocus(true); // Ceci permet à MainComponent d'obtenir le focus clavier

        juce::File svgFile {juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile).getSiblingFile("./Assets/drag_and_drop_wav.svg")};
        auto svgFileContent = svgFile.loadFileAsString();

        if (!svgFile.exists()) {
            DBG("File doesn't exist: " << svgFile.getFullPathName());
        } else {
            auto svgFileContent = svgFile.loadFileAsString();

            juce::XmlDocument xmlDoc(svgFileContent);
            std::unique_ptr<juce::XmlElement> xml(xmlDoc.getDocumentElement());

            if (xml != nullptr && xml->hasTagName("svg")) {
                svgDrawable = juce::Drawable::createFromSVG(*xml);

                if (svgDrawable != nullptr) {
                    addAndMakeVisible(svgDrawable.get());
                    svgDrawable->setTransformToFit(getLocalBounds().toFloat().withSizeKeepingCentre(400, 400), juce::RectanglePlacement::centred);
                } else {
                    DBG("Couldn't create Drawable from SVG.");
                }
            } else {
                DBG("XML is null or not an SVG.");
            }
        }

        addAndMakeVisible(fileLabel);
        addAndMakeVisible(titleLabel);
        addAndMakeVisible(channelsLabel);
        addAndMakeVisible(sampleRateLabel);
        addAndMakeVisible(durationLabel);
        addAndMakeVisible(bpmLabel);
        addAndMakeVisible(bpmEditor);
        addAndMakeVisible(barLabel);
        addAndMakeVisible(barEditor);
        addAndMakeVisible(closeButton);

        titleLabel.setFont(juce::Font (32.0f));
        titleLabel.setJustificationType (juce::Justification::centred);
        titleLabel.setText("Octatrack wav Split by bressani.dev", juce::dontSendNotification);
        titleLabel.setBounds(10, 20, getWidth() - 20, 40);

        durationLabel.setText("", juce::dontSendNotification);
        durationLabel.setBounds(10, getHeight() - 60, getWidth() - 20, 20);
        sampleRateLabel.setText("", juce::dontSendNotification);
        sampleRateLabel.setBounds(10, getHeight() - 90, getWidth() - 20, 20);
        channelsLabel.setText("", juce::dontSendNotification);
        channelsLabel.setBounds(10, getHeight() - 120, getWidth() - 20, 20);

        // fileLabel.setFont(juce::Font("Arial", 12.0f, juce::Font::plain));
        fileLabel.setBounds(10, getHeight() - 30, getWidth() - 20, 20);


        bpmLabel.setFont (juce::Font (30.0f));
        bpmLabel.setText("Bpm: ", juce::dontSendNotification);
        bpmLabel.setJustificationType (juce::Justification::centred);
        bpmEditor.setFont (juce::Font (30.0f));
        bpmEditor.setInputRestrictions(0, "0123456789."); // Autoriser uniquement les chiffres et le point.
        bpmEditor.setJustification(juce::Justification::centred);
        bpmEditor.setText("120.0");

        barLabel.setFont (juce::Font (30.0f));
        barLabel.setText("Bar: ", juce::dontSendNotification);
        barLabel.setJustificationType (juce::Justification::centred);
        barEditor.setFont (juce::Font (30.0f));
        barEditor.setInputRestrictions(0, "0123456789"); // Autoriser uniquement les chiffres et le point.
        barEditor.setJustification(juce::Justification::centred);
        barEditor.setText("32");

        closeButton.setBounds(getWidth() - 100, 10, 80, 30);

        closeButton.onClick = [this] { juce::JUCEApplication::getInstance()->systemRequestedQuit(); };
    }

    ~MainComponent() override
    {
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(getLookAndFeel().findColour(
                juce::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        if (svgDrawable != nullptr) {
            svgDrawable->setBounds(getLocalBounds().reduced(10));
        }
        fileLabel.setBounds(10, getHeight() - 30, getWidth() - 10, 20);
        channelsLabel.setBounds(10, getHeight() -60, getWidth() - 10, 20);
        sampleRateLabel.setBounds(10, getHeight() -90, getWidth() - 10, 20);
        durationLabel.setBounds(10, getHeight() -120, getWidth() - 10, 20);
        titleLabel.setBounds((getWidth() - 200) / 2, 20 /*(getHeight() - 30)*/ / 2, 400, 30);
        bpmLabel.setBounds(500, getHeight() - 120, 100, 40);
        bpmEditor.setBounds(600, getHeight() - 120, 150, 40);
        barLabel.setBounds(500, getHeight() - 60, 100, 40);
        barEditor.setBounds(600, getHeight() - 60, 150, 40);
        closeButton.setBounds(getWidth() - 100, 10, 80, 30);
    }

    bool isInterestedInFileDrag(const juce::StringArray &files) override {
        for(auto &file : files) {
            if (file.endsWith(".wav")) {
                return true;
            }
        }
        return false;
    }

    void filesDropped(const juce::StringArray &files, int x, int y) override {
        for(auto &file : files) {
            if (file.endsWith(".wav")) {
                try {
                    AudioFileProperties afp(file.toStdString());

                    fileLabel.setText(file, juce::dontSendNotification);
                    channelsLabel.setText("Channels: " + juce::String(afp.getChannels()) , juce::dontSendNotification);
                    sampleRateLabel.setText("Sample rate: " + juce::String(afp.getSampleRate()) + " @ " + juce::String(afp.getPcmBitDepth()) + "bits PCM", juce::dontSendNotification);
                    durationLabel.setText("Duration: " + juce::String(afp.getDuration(), 2) + " seconds", juce::dontSendNotification);
                    break;
                } catch (const std::runtime_error& e) {
                    DBG("Error opening file: " << e.what());
                    juce::JUCEApplication::getInstance()->systemRequestedQuit();
                }
            }
        }
    }

    bool keyPressed (const juce::KeyPress& key) override
    {
        if (key == juce::KeyPress::escapeKey)
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
            return true;  // indique que vous avez géré l'événement de touche
        }

        return false;  // laisse les autres événements de touche être gérés ailleurs
    }
private:
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
    std::unique_ptr<juce::Drawable> svgDrawable;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

class MainWindow : public juce::DocumentWindow {
public:
    MainWindow(juce::String name)
        : DocumentWindow(
              name,
              juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(
                  juce::ResizableWindow::backgroundColourId),
              DocumentWindow::allButtons)
    {
        setUsingNativeTitleBar(true);
        setContentOwned(new MainComponent(), true);

        setResizable(true, true);
        centreWithSize(getWidth(), getHeight());
        setVisible(true);
    }

    void closeButtonPressed() override
    {
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
    }

};

class MyApp : public juce::JUCEApplication {
public:
    const juce::String getApplicationName() override { return "OctaSplit"; }
    const juce::String getApplicationVersion() override { return "0.0.1"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const juce::String&) override
    {
        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override { mainWindow = nullptr; }

    void systemRequestedQuit() override { quit(); }

    void anotherInstanceStarted(const juce::String&) override {}

private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(MyApp)

