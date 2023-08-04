//
// Created by Stéphane on 04.08.23.
//

#pragma once
#include <sndfile.h>
#include <string>

class AudioFileProperties
{
public:
    AudioFileProperties(const std::string& filePath);
    ~AudioFileProperties();

    int getChannels() const;
    int getSampleRate() const;
    double getDuration() const;
    std::string getFilePath() const;
    int64_t getFrames() const;
    // Retourne la profondeur de bit du fichier PCM ou -1 si le fichier n'est pas PCM
    int getPcmBitDepth() const;

    void process16Bit(int framesPerBar, int totalBars);
    void process24Bit(int framesPerBar, int totalBars);
    void process32Bit(int framesPerBar, int totalBars);
    void splitByBars(float bpm, int bars);
private:
    SNDFILE* file;
    SF_INFO info;
    std::string filePath;
};
