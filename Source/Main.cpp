#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>

class MainComponent : public juce::Component {
public:
    MainComponent()
    {
        setSize(800, 600);

        // À ce stade, vous devrez charger votre fichier audio,
        // par exemple en utilisant juce::AudioFormatReader et
        // juce::AudioFormatManager, puis récupérer les informations
        // que vous voulez afficher.

        // Vous pouvez ensuite créer des juce::Label pour afficher
        // ces informations et les ajouter à votre composant.
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(getLookAndFeel().findColour(
            juce::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        // Positionner vos labels ici.
    }

private:
    // vos labels ici
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

