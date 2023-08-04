//
// Created by Stéphane on 04.08.23.
//

#include "AudioFileProperties.h"

AudioFileProperties::AudioFileProperties(const juce::String& filePath)
        : sh(filePath.toRawUTF8())
{
    if(sh.error()) {
        throw std::runtime_error(sh.strError());
    }
}

int AudioFileProperties::getChannels() const {
    return sh.channels();
}

int AudioFileProperties::getSampleRate() const {
    return sh.samplerate();
}

sf_count_t AudioFileProperties::getFrames() const {
    return sh.frames();
}

double AudioFileProperties::getDuration() const {
    return static_cast<double>(getFrames()) / static_cast<double>(getSampleRate());
}
