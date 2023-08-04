#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>

class MainComponent : public juce::Component {
public:
    MainComponent()
    {
        helloWorldLabel.setFont (juce::Font (32.0f));
        helloWorldLabel.setJustificationType (juce::Justification::centred);
        helloWorldLabel.setText ("Hello, World!", juce::dontSendNotification);

        closeButton.setButtonText ("Fermer");
        closeButton.onClick = [this] { juce::JUCEApplication::getInstance()->systemRequestedQuit(); };

        addAndMakeVisible (helloWorldLabel);
        addAndMakeVisible (closeButton);

        setSize (800, 600);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(getLookAndFeel().findColour(
            juce::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        helloWorldLabel.setBounds (bounds.removeFromTop (bounds.getHeight() / 2));
        closeButton.setBounds (bounds.removeFromTop (bounds.getHeight() / 2));
    }

private:
    juce::Label helloWorldLabel;
    juce::TextButton closeButton;

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

