//
// Created by Stéphane on 04.08.23.
//

#include "AudioFileProperties.h"
#include <iostream>
#include <stdexcept>

AudioFileProperties::AudioFileProperties(const std::string& filePath) : filePath(filePath) {
    file = sf_open(filePath.c_str(), SFM_READ, &info);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }
}

AudioFileProperties::~AudioFileProperties() {
    sf_close(file);
}

int AudioFileProperties::getChannels() const {
    return info.channels;
}

int AudioFileProperties::getSampleRate() const {
    return info.samplerate;
}

double AudioFileProperties::getDuration() const {
    return static_cast<double>(getFrames()) / getSampleRate();
}

std::string AudioFileProperties::getFilePath() const {
    return filePath;
}

int64_t AudioFileProperties::getFrames() const {
    return info.frames;
}

int AudioFileProperties::getPcmBitDepth() const {
    if ((info.format & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
        // Ce n'est pas un fichier WAV, donc on ne peut pas déterminer la profondeur de bit PCM
        return -1;
    }

    switch (info.format & SF_FORMAT_SUBMASK) {
        case SF_FORMAT_PCM_16:
            return 16;
        case SF_FORMAT_PCM_24:
            return 24;
        case SF_FORMAT_PCM_32:
            return 32;
        default:
            // Ce n'est pas un format PCM, donc on ne peut pas déterminer la profondeur de bit
            return -1;
    }
}

void AudioFileProperties::splitByBars(float bpm, int bars) {
    // Calculer le nombre de frames par mesure
    double secondsPerBeat = 60.0 / bpm;
    int framesPerBar = secondsPerBeat * bars * getSampleRate();

    // Calculer le nombre total de mesures
    int totalBars = getFrames() / framesPerBar;

    // Préparer un buffer pour la lecture des frames
    std::vector<short> buffer(framesPerBar * getChannels());

    for (int bar = 0; bar < totalBars; ++bar) {
        // Lire les frames de la mesure actuelle
        sf_seek(file, bar * framesPerBar, SEEK_SET);
        sf_readf_short(file, buffer.data(), framesPerBar);

        // Préparer le nom du fichier de sortie
        std::string outFileName = getFilePath() + "_" + std::to_string(bar) + ".wav";

        // Ouvrir le fichier de sortie
        SF_INFO outFileInfo = info;
        outFileInfo.frames = framesPerBar;
        SNDFILE* outFile = sf_open(outFileName.c_str(), SFM_WRITE, &outFileInfo);

        if (!outFile) {
            std::cout << "Cannot open output file: " << outFileName << std::endl;
            continue;
        }

        // Écrire les frames dans le fichier de sortie
        sf_writef_short(outFile, buffer.data(), framesPerBar);

        // Fermer le fichier de sortie
        sf_close(outFile);
    }
}
