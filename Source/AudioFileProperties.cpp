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
/*
void AudioFileProperties::splitByBars(float bpm, int bars) {
    // Calculer le nombre de frames par mesure
    double secondsPerBeat = 60.0 / bpm;
    int framesPerBar = secondsPerBeat * (bars * 4) * getSampleRate();

    // Calculer le nombre total de mesures
    int totalBars = getFrames() / framesPerBar;

    // Préparer un buffer pour la lecture des frames
    // We use a buffer of floats as it can accommodate all PCM data formats
    std::vector<float> buffer(framesPerBar * getChannels());

    for (int bar = 0; bar < totalBars; ++bar) {
        // Lire les frames de la mesure actuelle
        sf_seek(file, bar * framesPerBar, SEEK_SET);

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

        // Check PCM format and read/write data accordingly
        switch(getPcmBitDepth()) {
            case 16:
                sf_readf_short(file, reinterpret_cast<short*>(buffer.data()), framesPerBar);
                sf_writef_short(outFile, reinterpret_cast<short*>(buffer.data()), framesPerBar);
                break;
            case 24:
                sf_readf_int(file, reinterpret_cast<int*>(buffer.data()), framesPerBar);
                sf_writef_int(outFile, reinterpret_cast<int*>(buffer.data()), framesPerBar);
                break;
            case 32:
                sf_readf_float(file, buffer.data(), framesPerBar);
                sf_writef_float(outFile, buffer.data(), framesPerBar);
                break;
            default:
                std::cout << "Unsupported PCM format: " << getPcmBitDepth() << std::endl;
                sf_close(outFile);
                return;
        }

        // Fermer le fichier de sortie
        sf_close(outFile);
    }
}*/

void AudioFileProperties::process16Bit(int framesPerBar, int totalBars) {
    // Process 16-bit PCM data...

    // Préparer le nom du fichier de sortie
    std::string outFileName;

    // Ouvrir le fichier de sortie
    SF_INFO outFileInfo;
    SNDFILE* outFile;

    std::vector<short> buffer(framesPerBar * getChannels());

    for (int bar = 0; bar < totalBars; ++bar) {
        // Lire les frames de la mesure actuelle
        sf_seek(file, bar * framesPerBar, SEEK_SET);
        sf_readf_short(file, buffer.data(), framesPerBar);

        outFileName = getFilePath() + "_" + std::to_string(bar) + ".wav";
        outFileInfo = info;
        outFileInfo.frames = framesPerBar;
        outFile = sf_open(outFileName.c_str(), SFM_WRITE, &outFileInfo);

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

void AudioFileProperties::process24Bit(int framesPerBar, int totalBars) {
    // Process 24-bit PCM data...

    // Préparer le nom du fichier de sortie
    std::string outFileName;

    // Ouvrir le fichier de sortie
    SF_INFO outFileInfo;
    SNDFILE* outFile;

    std::vector<int> buffer(framesPerBar * getChannels());

    for (int bar = 0; bar < totalBars; ++bar) {
        // Lire les frames de la mesure actuelle
        sf_seek(file, bar * framesPerBar, SEEK_SET);
        sf_readf_int(file, buffer.data(), framesPerBar);

        outFileName = getFilePath() + "_" + std::to_string(bar) + ".wav";
        outFileInfo = info;
        outFileInfo.frames = framesPerBar;
        outFile = sf_open(outFileName.c_str(), SFM_WRITE, &outFileInfo);

        if (!outFile) {
            std::cout << "Cannot open output file: " << outFileName << std::endl;
            continue;
        }

        // Écrire les frames dans le fichier de sortie
        sf_writef_int(outFile, buffer.data(), framesPerBar);

        // Fermer le fichier de sortie
        sf_close(outFile);
    }

}

void AudioFileProperties::process32Bit(int framesPerBar, int totalBars) {
    // Process 32-bit PCM data...

    // Préparer le nom du fichier de sortie
    std::string outFileName;

    // Ouvrir le fichier de sortie
    SF_INFO outFileInfo;
    SNDFILE* outFile;

    std::vector<float> buffer(framesPerBar * getChannels());

    for (int bar = 0; bar < totalBars; ++bar) {
        // Lire les frames de la mesure actuelle
        sf_seek(file, bar * framesPerBar, SEEK_SET);
        sf_readf_float(file, buffer.data(), framesPerBar);

        outFileName = getFilePath() + "_" + std::to_string(bar) + ".wav";
        outFileInfo = info;
        outFileInfo.frames = framesPerBar;
        outFile = sf_open(outFileName.c_str(), SFM_WRITE, &outFileInfo);

        if (!outFile) {
            std::cout << "Cannot open output file: " << outFileName << std::endl;
            continue;
        }

        // Écrire les frames dans le fichier de sortie
        sf_writef_float(outFile, buffer.data(), framesPerBar);

        // Fermer le fichier de sortie
        sf_close(outFile);
    }

}

void AudioFileProperties::splitByBars(float bpm, int bars) {
    // Calculer le nombre de frames par mesure
    double secondsPerBeat = 60.0 / bpm;
    double exactFramesPerBar = secondsPerBeat * (bars * 4) * getSampleRate();
    int framesPerBar = static_cast<int>(exactFramesPerBar);

    // Calculer le nombre total de mesures
    int totalBars = getFrames() / framesPerBar;

    // Check PCM format and read/write data accordingly
    switch(getPcmBitDepth()) {
        case 16:
            process16Bit(framesPerBar, totalBars);
            break;
        case 24:
            process24Bit(framesPerBar, totalBars);
            break;
        case 32:
            process32Bit(framesPerBar, totalBars);
            break;
        default:
            std::cout << "Unsupported PCM format: " << getPcmBitDepth() << std::endl;
            return;
    }
}
