//
// Created by Stéphane on 04.08.23.
//

#pragma once

#include <sndfile.hh>
#include <juce_audio_formats/juce_audio_formats.h>

class AudioFileProperties
{
public:
    AudioFileProperties(const juce::String& filePath);

    int getChannels() const;
    int getSampleRate() const;
    sf_count_t getFrames() const;
    double getDuration() const;

private:
    SndfileHandle sh;
};
