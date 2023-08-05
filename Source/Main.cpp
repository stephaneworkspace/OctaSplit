#include <vector>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "AudioFileProperties.h"

using namespace juce;

class MainComponent : public juce::Component,
                      public juce::KeyListener,
                      public juce::TextEditor::Listener,
                      public juce::DragAndDropContainer,
                      public juce::FileDragAndDropTarget,
                      private juce::Timer {
public:
    MainComponent()
            : fileLabel("", "No file loaded..."),
              closeButton("Close"),
              splitButton("Split"),
              fileSelectButton("Select .wav file"),
              bpmEditor(),
              barEditor()
    {
        setSize(800, 600);
        startTimerHz(60); // Change this value to control the frame rate
        setWantsKeyboardFocus(true); // Ceci permet à MainComponent d'obtenir le focus clavier

        juce::File svgFile1 {juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile).getSiblingFile("./Assets/logobg.svg")};
        auto svgFileContent1 = svgFile1.loadFileAsString();

        if (!svgFile1.exists()) {
            DBG("File doesn't exist: " << svgFile1.getFullPathName());
        } else {
            auto svgFileContent1 = svgFile1.loadFileAsString();

            juce::XmlDocument xmlDoc(svgFileContent1);
            std::unique_ptr<juce::XmlElement> xml(xmlDoc.getDocumentElement());

            if (xml != nullptr && xml->hasTagName("svg")) {
                svgDrawable1 = juce::Drawable::createFromSVG(*xml);

                if (svgDrawable1 != nullptr) {
                    addAndMakeVisible(svgDrawable1.get());
                    svgDrawable1->setTransformToFit(getLocalBounds().toFloat().withSizeKeepingCentre(600, 600), juce::RectanglePlacement::centred);
                } else {
                    DBG("Couldn't create Drawable from SVG.");
                }
            } else {
                DBG("XML is null or not an SVG.");
            }
        }

        juce::File svgFile2 {juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile).getSiblingFile("./Assets/logo.svg")};
        auto svgFileContent2 = svgFile2.loadFileAsString();

        if (!svgFile2.exists()) {
            DBG("File doesn't exist: " << svgFile2.getFullPathName());
        } else {
            auto svgFileContent2 = svgFile2.loadFileAsString();

            juce::XmlDocument xmlDoc(svgFileContent2);
            std::unique_ptr<juce::XmlElement> xml(xmlDoc.getDocumentElement());

            if (xml != nullptr && xml->hasTagName("svg")) {
                svgDrawable2 = juce::Drawable::createFromSVG(*xml);

                if (svgDrawable2 != nullptr) {
                    //addAndMakeVisible(svgDrawable2.get());
                    svgDrawable2->setTransformToFit(getLocalBounds().toFloat().withSizeKeepingCentre(300, 300), juce::RectanglePlacement::centred);
                } else {
                    DBG("Couldn't create Drawable from SVG.");
                }
            } else {
                DBG("XML is null or not an SVG.");
            }
        }

        juce::File svgFile3 {juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile).getSiblingFile("./Assets/bressani.dev.svg")};
        auto svgFileContent3 = svgFile3.loadFileAsString();

        if (!svgFile3.exists()) {
            DBG("File doesn't exist: " << svgFile3.getFullPathName());
        } else {
            auto svgFileContent3 = svgFile3.loadFileAsString();

            juce::XmlDocument xmlDoc(svgFileContent3);
            std::unique_ptr<juce::XmlElement> xml(xmlDoc.getDocumentElement());

            if (xml != nullptr && xml->hasTagName("svg")) {
                svgDrawable3 = juce::Drawable::createFromSVG(*xml);

                if (svgDrawable3 != nullptr) {
                    svgDrawable3->replaceColour(juce::Colours::black, juce::Colours::white);
                    addAndMakeVisible(svgDrawable3.get());
                    svgDrawable3->setTransformToFit(getLocalBounds().toFloat().withSizeKeepingCentre(600, 600), juce::RectanglePlacement::centred);
                } else {
                    DBG("Couldn't create Drawable from SVG.");
                }
            } else {
                DBG("XML is null or not an SVG.");
            }
        }


        bpmEditor.addListener(this);
        barEditor.addListener(this);

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
        addAndMakeVisible(splitButton);
        addAndMakeVisible(fileSelectButton);

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
        splitButton.setBounds(getWidth() - 100, 90, 80, 30);
        splitButton.onClick = [this] {
            if (bpmEditor.getText().isEmpty() || barEditor.getText().isEmpty()) {
                return;
            }

            float bpm = bpmEditor.getText().getFloatValue();
            int bars = barEditor.getText().getIntValue();

            try {
                AudioFileProperties afp(fileLabel.getText().toStdString());
                afp.splitByBars(bpm, bars);
                durationLabel.setText ("", dontSendNotification);
                sampleRateLabel.setText ("", dontSendNotification);
                channelsLabel.setText ("", dontSendNotification);
                fileLabel.setText ("", dontSendNotification);
            } catch (const std::runtime_error& e) {
                DBG("Error opening file: " << e.what());
                juce::NativeMessageBox::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Error", "Error opening file: " + juce::String(e.what()));
               // juce::JUCEApplication::getInstance()->systemRequestedQuit();
            }
        };
        fileSelectButton.setButtonText("Select .wav file");
        fileSelectButton.setBounds(getWidth() - 100, 50, 80, 30);
        fileSelectButton.onClick = [this] { fileSelectButtonClicked(); };
    }

    ~MainComponent() override
    {
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(getLookAndFeel().findColour(
                juce::ResizableWindow::backgroundColourId));
        if (svgDrawable2)
        {
            svgDrawable2->draw(g, 1.0f,
                               juce::AffineTransform::rotation(rotationAngle,
                                                               getLocalBounds().getCentreX(),
                                                               getLocalBounds().getCentreY()));
        }
    }

    void update()
    {
        rotationAngle += 0.01f; // Change this value to control the speed of rotation
        if (rotationAngle > juce::MathConstants<float>::twoPi)
            rotationAngle -= juce::MathConstants<float>::twoPi;
        repaint();
    }

    void resized() override
    {
        if (svgDrawable1 != nullptr) {
            svgDrawable1->setBounds(getLocalBounds().reduced(10));
        }
        //if (svgDrawable2 != nullptr) {
        //    svgDrawable2->setBounds(getLocalBounds().reduced(10));
        //}
        if (svgDrawable3 != nullptr) {
            svgDrawable3->setBounds(getLocalBounds().reduced(10));
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
        fileSelectButton.setBounds(getWidth() - 100, 50, 80, 30);
        splitButton.setBounds(getWidth() - 100, 90, 80, 30);
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
                // Vérification si le fichier est valide
                if (files.size() == 1)
                {
                    File audioFile(files[0]);

                    SF_INFO sfInfo;
                    sfInfo.format = 0; // Laissez la libsndfile déterminer le format

                    SNDFILE* file = sf_open(audioFile.getFullPathName().toRawUTF8(), SFM_READ, &sfInfo);

                    if (file == nullptr)
                    {
                        // Si le fichier n'est pas un fichier WAV valide, affichez un message d'erreur et réinitialisez le champ fileLabel.
                        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                          "Invalid file",
                                                          "The selected file is not a valid WAV file.");
                        fileLabel.setText ("", dontSendNotification);
                    }
                    else
                    {
                        if ((sfInfo.format & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV)
                        {
                            // Le fichier n'est pas au format WAV
                            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                              "Invalid file",
                                                              "The selected file is not a valid WAV file.");
                            fileLabel.setText ("", dontSendNotification);
                        }
                        // Le fichier est valide, vous pouvez continuer avec votre traitement.
                        // N'oubliez pas de fermer le fichier lorsque vous avez terminé avec lui.
                        sf_close(file);

                        // Votre traitement du fichier ici...
                    }
                }

                try {
                    AudioFileProperties afp(file.toStdString());

                    fileLabel.setText(file, juce::dontSendNotification);
                    channelsLabel.setText("Channels: " + juce::String(afp.getChannels()) , juce::dontSendNotification);
                    sampleRateLabel.setText("Sample rate: " + juce::String(afp.getSampleRate()) + " @ " + juce::String(afp.getPcmBitDepth()) + "bits PCM", juce::dontSendNotification);
                    durationLabel.setText("Duration: " + juce::String(afp.getDuration(), 2) + " seconds", juce::dontSendNotification);
                    break;
                } catch (const std::runtime_error& e) {
                    DBG("Error opening file: " << e.what());
                    juce::NativeMessageBox::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Error", "Error opening file: " + juce::String(e.what()));
                    //juce::JUCEApplication::getInstance()->systemRequestedQuit();
                }
            }
        }
    }

    bool keyPressed(const juce::KeyPress &key, juce::Component *originatingComponent) override {
        if (key == juce::KeyPress::escapeKey) {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
            return true;  // Indiquer que l'événement de la touche a été traité
        }
        return false;  // Indiquer que l'événement de la touche n'a pas été traité
    }

    void textEditorTextChanged (juce::TextEditor& editor) override
    {
        splitButton.setEnabled(!bpmEditor.getText().isEmpty() && !barEditor.getText().isEmpty());
    }

    void fileSelectButtonClicked()
    {
        juce::FileChooser chooser("Select a .wav file", juce::File{}, "*.wav");

        if (chooser.browseForFileToOpen())
        {
            auto file = chooser.getResult();
            // Vous pouvez utiliser 'file' ici comme vous le souhaitez.
            // Par exemple, vous pouvez l'utiliser pour mettre à jour vos étiquettes, comme dans votre méthode `filesDropped`.
            juce::StringArray files;
            files.add(file.getFullPathName());
            filesDropped(files, 0, 0); // Vous pouvez ajuster les coordonnées x et y si nécessair
        }
    }

private:
    void timerCallback() override
    {
        update();
    }

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

        //setResizable(true, true);
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

