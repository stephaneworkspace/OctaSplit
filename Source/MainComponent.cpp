//
// Created by Stéphane on 05.08.23.
//
#include "MainComponent.h"

MainComponent::MainComponent() : fileLabel("", "No file loaded..."),
                                 closeButton("Close"),
                                 splitButton("Split"),
                                 fileSelectButton("Select .wav file"),
                                 aboutButton("About"),
                                 bpmEditor(),
                                 barEditor() {
    // Open .env

    Misc misc;
    appVersion = misc.getAppVersion();

    // JUCE settings
    setSize(800, 600);
    startTimerHz(60); // Change this value to control the frame rate
    setWantsKeyboardFocus(true); // Ceci permet à MainComponent d'obtenir le focus clavier

    loadPNG("./Assets/bg.png");
    // Loading SVGs
    svgDrawable1 = loadSVG("./Assets/logobg.svg");
    if (svgDrawable1 != nullptr) {
        addAndMakeVisible(svgDrawable1.get());
        svgDrawable1->setTransformToFit(getLocalBounds().toFloat().withSizeKeepingCentre(600, 600), juce::RectanglePlacement::centred);
    }
    svgDrawable2 = loadSVG("./Assets/logo.svg");
    if (svgDrawable2 != nullptr) {
        svgDrawable2->setTransformToFit(getLocalBounds().toFloat().withSizeKeepingCentre(300, 300), juce::RectanglePlacement::centred);
    }
    svgDrawable3 = loadSVG("./Assets/bressani.dev.svg");
    if (svgDrawable3 != nullptr) {
        svgDrawable3->replaceColour(juce::Colours::black, juce::Colours::white);
        addAndMakeVisible(svgDrawable3.get());
        svgDrawable3->setTransformToFit(getLocalBounds().toFloat().withSizeKeepingCentre(600, 600), juce::RectanglePlacement::centred);
    }

    bpmEditor.addListener(this);
    barEditor.addListener(this);

    addAndMakeVisible(fileLabel);
    //addAndMakeVisible(titleLabel);
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
    addAndMakeVisible(aboutButton);

    //titleLabel.setFont(juce::Font (32.0f));
    //titleLabel.setJustificationType (juce::Justification::centred);
    //titleLabel.setText("Octatrack wav Split", juce::dontSendNotification);
    //titleLabel.setBounds(10, 20, getWidth() - 20, 40);

    durationLabel.setText("", juce::dontSendNotification);
    durationLabel.setBounds(10, getHeight() - 60, getWidth() - 20, 20);
    sampleRateLabel.setText("", juce::dontSendNotification);
    sampleRateLabel.setBounds(10, getHeight() - 90, getWidth() - 20, 20);
    channelsLabel.setText("", juce::dontSendNotification);
    channelsLabel.setBounds(10, getHeight() - 120, getWidth() - 20, 20);

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
    splitButton.onClick = [this] { splitButtonClicked(); };
    fileSelectButton.setButtonText("Select .wav file");
    fileSelectButton.setBounds(getWidth() - 100, 50, 80, 30);
    fileSelectButton.onClick = [this] { fileSelectButtonClicked(); };
    aboutButton.setButtonText("About");
    aboutButton.setBounds(getWidth() - 100, 130, 80, 30);
    aboutButton.onClick = [this] { aboutButtonClicked(); };
}

MainComponent::~MainComponent() { }

void MainComponent::paint(Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(
            juce::ResizableWindow::backgroundColourId));
    g.drawImageAt(pngImage, 0, 0); // Dessine l'image à la position souhaitée
    if (svgDrawable2)
    {
        svgDrawable2->draw(g, 1.0f,
                           juce::AffineTransform::rotation(rotationAngle,
                                                           getLocalBounds().getCentreX(),
                                                           getLocalBounds().getCentreY()));
    }
}

void MainComponent::update()
{
    rotationAngle += 0.01f; // Change this value to control the speed of rotation
    if (rotationAngle > juce::MathConstants<float>::twoPi)
        rotationAngle -= juce::MathConstants<float>::twoPi;
    repaint();
}

void MainComponent::resized()
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
    //titleLabel.setBounds((getWidth() - 200) / 2, 20 /*(getHeight() - 30)*/ / 2, 400, 30);
    bpmLabel.setBounds(500, getHeight() - 120, 100, 40);
    bpmEditor.setBounds(600, getHeight() - 120, 150, 40);
    barLabel.setBounds(500, getHeight() - 60, 100, 40);
    barEditor.setBounds(600, getHeight() - 60, 150, 40);
    closeButton.setBounds(getWidth() - 100, 10, 80, 30);
    fileSelectButton.setBounds(getWidth() - 100, 50, 80, 30);
    splitButton.setBounds(getWidth() - 100, 90, 80, 30);
    aboutButton.setBounds(getWidth() - 100, 140, 80, 30);
}

bool MainComponent::isInterestedInFileDrag(const juce::StringArray &files) {
    for(auto &file : files) {
        if (file.endsWith(".wav")) {
            return true;
        }
    }
    return false;
}

void MainComponent::filesDropped(const juce::StringArray &files, int x, int y) {
    for(auto &file : files) {
        if (file.endsWith(".wav")) {
            // Vérification si le fichier est valide
            if (files.size() == 1)
            {
                File audioFile(files[0]);

                SF_INFO sfInfo;
                sfInfo.format = 0; // Laissez la libsndfile déterminer le format

                SNDFILE* f = sf_open(audioFile.getFullPathName().toRawUTF8(), SFM_READ, &sfInfo);

                if (f == nullptr)
                {
                    // Si le fichier n'est pas un fichier WAV valide, affichez un message d'erreur et réinitialisez le champ fileLabel.
                    AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                      "Invalid file",
                                                      "The selected file is not a valid WAV file.");
                    fileLabel.setText ("No file loaded...", dontSendNotification);
                    durationLabel.setText ("", dontSendNotification);
                    sampleRateLabel.setText ("", dontSendNotification);
                    channelsLabel.setText ("", dontSendNotification);
                }
                else
                {
                    if ((sfInfo.format & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV)
                    {
                        // Le fichier n'est pas au format WAV
                        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                          "Invalid file",
                                                          "The selected file is not a valid WAV file.");
                        fileLabel.setText ("No file loaded...", dontSendNotification);
                        durationLabel.setText ("", dontSendNotification);
                        sampleRateLabel.setText ("", dontSendNotification);
                        channelsLabel.setText ("", dontSendNotification);
                    }
                    // Le fichier est valide, vous pouvez continuer avec votre traitement.
                    // N'oubliez pas de fermer le fichier lorsque vous avez terminé avec lui.
                    sf_close(f);

                    // Votre traitement du fichier ici...
                    try {
                        AudioFileProperties afp(file.toStdString());

                        fileLabel.setText(file, juce::dontSendNotification);
                        channelsLabel.setText("Channels: " + juce::String(afp.getChannels()) , juce::dontSendNotification);
                        sampleRateLabel.setText("Sample rate: " + juce::String(afp.getSampleRate()) + " @ " + juce::String(afp.getPcmBitDepth()) + "bits PCM", juce::dontSendNotification);
                        durationLabel.setText("Duration: " + juce::String(afp.getDuration(), 2) + " seconds", juce::dontSendNotification);
                        break;
                    } catch (const std::runtime_error& e) {
                        // DBG("Error opening file: " << e.what());
                        // juce::NativeMessageBox::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Error", "Error opening file: " + juce::String(e.what()));
                        // juce::JUCEApplication::getInstance()->systemRequestedQuit();
                        // Le fichier n'est pas au format WAV
                        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                          "Error",
                                                          "Can't open file.");
                        fileLabel.setText ("No file loaded...", dontSendNotification);
                    }
                }
            }
        }
    }
}

bool MainComponent::keyPressed(const juce::KeyPress &key, juce::Component *originatingComponent) {
    if (key == juce::KeyPress::escapeKey) {
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
        return true;  // Indiquer que l'événement de la touche a été traité
    }
    return false;  // Indiquer que l'événement de la touche n'a pas été traité
}

void MainComponent::textEditorTextChanged (juce::TextEditor& editor)
{
    splitButton.setEnabled(!bpmEditor.getText().isEmpty() && !barEditor.getText().isEmpty());
}

void MainComponent::fileSelectButtonClicked()
{
    FileChooser chooser("Select a .wav file", File{}, "*.wav");

    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        // Vous pouvez utiliser 'file' ici comme vous le souhaitez.
        // Par exemple, vous pouvez l'utiliser pour mettre à jour vos étiquettes, comme dans votre méthode `filesDropped`.
        StringArray files;
        files.add(file.getFullPathName());
        filesDropped(files, 0, 0); // Vous pouvez ajuster les coordonnées x et y si nécessair
    }
}

void MainComponent::splitButtonClicked()
{
    if (bpmEditor.getText().isEmpty() || barEditor.getText().isEmpty()) {
        return;
    }

    float bpm = bpmEditor.getText().getFloatValue();
    int bars = barEditor.getText().getIntValue();

    try {
        AudioFileProperties afp(fileLabel.getText().toStdString());
        afp.splitByBars(bpm, bars);
        AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                          "Info",
                                          "File successfully split !");
        durationLabel.setText ("", dontSendNotification);
        sampleRateLabel.setText ("", dontSendNotification);
        channelsLabel.setText ("", dontSendNotification);
        fileLabel.setText ("No file loaded...", dontSendNotification);
    } catch (const std::runtime_error& e) {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "Error",
                                          "Can't open file.");
        fileLabel.setText ("No file loaded...", dontSendNotification);
        durationLabel.setText ("", dontSendNotification);
        sampleRateLabel.setText ("", dontSendNotification);
        channelsLabel.setText ("", dontSendNotification);
    }
}

void MainComponent::aboutButtonClicked()
{
    AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                      "About",
                                      "This freeware is made by Stephane Bressani - www.bressani.dev\nVersion: " + appVersion);
}

void MainComponent::timerCallback()
{
    update();
}

void MainComponent::loadPNG(const juce::String& path)
{
    auto inputStream = std::make_unique<juce::FileInputStream>(juce::File(path));
    pngImage = juce::PNGImageFormat().decodeImage(*inputStream);
}

unique_ptr<Drawable> MainComponent::loadSVG(const String &path) {
    File svgFile{File::getSpecialLocation(File::SpecialLocationType::currentExecutableFile).getSiblingFile(path)};
    if (!svgFile.exists()) {
        DBG("File doesn't exist: " << svgFile.getFullPathName());
        return nullptr;
    }

    auto svgFileContent = svgFile.loadFileAsString();
    XmlDocument xmlDoc(svgFileContent);
    unique_ptr<XmlElement> xml(xmlDoc.getDocumentElement());

    if (xml != nullptr && xml->hasTagName("svg")) {
        return Drawable::createFromSVG(*xml);
    } else {
        DBG("XML is null or not an SVG.");
        return nullptr;
    }
}
