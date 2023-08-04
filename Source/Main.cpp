#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>

class MainComponent : public juce::Component, public juce::DragAndDropContainer, public juce::FileDragAndDropTarget {
public:
    MainComponent()
            : fileLabel("", "No file loaded..."),
              helloWorldLabel("", "Hello, World!"),
              closeButton("Close")
    {
        setSize(800, 600);

        //juce::File svgFile {"./Assets/drag_and_drop_wav.svg"};
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
        addAndMakeVisible(helloWorldLabel);
        addAndMakeVisible(closeButton);

        helloWorldLabel.setFont (juce::Font (32.0f));
        helloWorldLabel.setJustificationType (juce::Justification::centred);
        helloWorldLabel.setText ("Hello, World!", juce::dontSendNotification);

        fileLabel.setBounds(10, getHeight() - 30, getWidth() - 20, 20);
        closeButton.setBounds(getWidth() - 100, 10, 80, 30);

        closeButton.onClick = [this] { juce::JUCEApplication::getInstance()->systemRequestedQuit(); };
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
        fileLabel.setBounds(10, getHeight() - 30, getWidth() - 20, 20);
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
                fileLabel.setText(file, juce::dontSendNotification);
                break;
            }
        }
    }
private:
    juce::Label fileLabel;
    juce::Label helloWorldLabel;
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

