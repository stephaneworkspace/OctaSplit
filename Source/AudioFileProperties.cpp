//
// Created by Stéphane on 04.08.23.
//

#include "AudioFileProperties.h"

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

std::string AudioFileProperties::getFilePathWithoutExt() const {
    std::regex regex("\\.wav$", std::regex_constants::icase);
    return std::regex_replace(filePath, regex, "");
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

void AudioFileProperties::process16Bit(int framesPerBar, int totalBars) {
    // Process 16-bit PCM data...

    // Reste
    const int64_t remainder = getFrames() - (framesPerBar * totalBars);

    // Préparer le nom du fichier de sortie
    std::string outFileName;

    // Ouvrir le fichier de sortie
    SF_INFO outFileInfo;
    SNDFILE* outFile;

    std::vector<short> buffer(framesPerBar * getChannels());
    std::vector<short> buffer_remainder(remainder * getChannels());

    for (int bar = 0; bar < totalBars; ++bar) {
        // Lire les frames de la mesure actuelle
        sf_seek(file, bar * framesPerBar, SEEK_SET);
        sf_readf_short(file, buffer.data(), framesPerBar);

        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << bar;
        outFileName = getFilePathWithoutExt() + "_" + ss.str() + ".wav";
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
    if (remainder > 0) {
        int bar = totalBars;
        // Lire les frames de la mesure actuelle
        sf_seek(file, framesPerBar * totalBars, SEEK_SET);
        sf_readf_short(file, buffer_remainder.data(), remainder);

        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << bar;
        outFileName = getFilePathWithoutExt() + "_" + ss.str() + ".wav";
        outFileInfo = info;
        outFileInfo.frames = remainder;
        outFile = sf_open(outFileName.c_str(), SFM_WRITE, &outFileInfo);

        if (!outFile) {
            std::cout << "Cannot open output file: " << outFileName << std::endl;
        } else {
            // Écrire les frames dans le fichier de sortie
            sf_writef_short(outFile, buffer_remainder.data(), remainder);
            // Fermer le fichier de sortie
            sf_close(outFile);
        }
    }
}

void AudioFileProperties::process24Bit(int framesPerBar, int totalBars) {
    // Process 24-bit PCM data...

    // Reste
    const int64_t remainder = getFrames() - (framesPerBar * totalBars);

    // Préparer le nom du fichier de sortie
    std::string outFileName;

    // Ouvrir le fichier de sortie
    SF_INFO outFileInfo;
    SNDFILE* outFile;

    std::vector<int> buffer(framesPerBar * getChannels());
    std::vector<int> buffer_remainder(remainder * getChannels());

    for (int bar = 0; bar < totalBars; ++bar) {
        // Lire les frames de la mesure actuelle
        sf_seek(file, bar * framesPerBar, SEEK_SET);
        sf_readf_int(file, buffer.data(), framesPerBar);

        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << bar;
        outFileName = getFilePathWithoutExt() + "_" + ss.str() + ".wav";
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
    if (remainder > 0) {
        int bar = totalBars;
        // Lire les frames de la mesure actuelle
        sf_seek(file, framesPerBar * totalBars, SEEK_SET);
        sf_readf_int(file, buffer_remainder.data(), remainder);

        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << bar;
        outFileName = getFilePathWithoutExt() + "_" + ss.str() + ".wav";
        outFileInfo = info;
        outFileInfo.frames = remainder;
        outFile = sf_open(outFileName.c_str(), SFM_WRITE, &outFileInfo);

        if (!outFile) {
            std::cout << "Cannot open output file: " << outFileName << std::endl;
        } else {
            // Écrire les frames dans le fichier de sortie
            sf_writef_int(outFile, buffer_remainder.data(), remainder);
            // Fermer le fichier de sortie
            sf_close(outFile);
        }
    }
}

void AudioFileProperties::process32Bit(int framesPerBar, int totalBars) {
    // Process 32-bit PCM data...

    // Reste
    const int64_t remainder = getFrames() - (framesPerBar * totalBars);

    // Préparer le nom du fichier de sortie
    std::string outFileName;

    // Ouvrir le fichier de sortie
    SF_INFO outFileInfo;
    SNDFILE* outFile;

    std::vector<float> buffer(framesPerBar * getChannels());
    std::vector<float> buffer_remainder(remainder * getChannels());

    for (int bar = 0; bar < totalBars; ++bar) {
        // Lire les frames de la mesure actuelle
        sf_seek(file, bar * framesPerBar, SEEK_SET);
        sf_readf_float(file, buffer.data(), framesPerBar);

        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << bar;
        outFileName = getFilePathWithoutExt() + "_" + ss.str() + ".wav";
        //outFileName = getFilePathWithoutExt() + "_" + std::to_string(bar) + ".wav";
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
    if (remainder > 0) {
        int bar = totalBars;
        // Lire les frames de la mesure actuelle
        sf_seek(file, framesPerBar * totalBars, SEEK_SET);
        sf_readf_float(file, buffer_remainder.data(), remainder);

        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << bar;
        outFileName = getFilePathWithoutExt() + "_" + ss.str() + ".wav";
        outFileInfo = info;
        outFileInfo.frames = remainder;
        outFile = sf_open(outFileName.c_str(), SFM_WRITE, &outFileInfo);

        if (!outFile) {
            std::cout << "Cannot open output file: " << outFileName << std::endl;
        } else {
            // Écrire les frames dans le fichier de sortie
            sf_writef_float(outFile, buffer_remainder.data(), remainder);
            // Fermer le fichier de sortie
            sf_close(outFile);
        }
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
