#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "Misc.h"
#include "MainComponent.h"

using namespace std;
using namespace juce;

class MainWindow : public DocumentWindow {
public:
    MainWindow(String name)
        : DocumentWindow(
              name,
              Desktop::getInstance().getDefaultLookAndFeel().findColour(
                  ResizableWindow::backgroundColourId),
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
        JUCEApplication::getInstance()->systemRequestedQuit();
    }

};

class MyApp : public JUCEApplication {
public:
    MyApp() {
        // Open .env
        Misc misc;
        appName = misc.getAppName();
        appVersion = misc.getAppVersion();
    }
    const String getApplicationName() override { return String(appName); }
    const String getApplicationVersion() override { return String(appVersion); }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const String&) override
    {
        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override { mainWindow = nullptr; }

    void systemRequestedQuit() override { quit(); }

    void anotherInstanceStarted(const String&) override {}

private:
    unique_ptr<MainWindow> mainWindow;
    string appName;
    string appVersion;
};

START_JUCE_APPLICATION(MyApp)

