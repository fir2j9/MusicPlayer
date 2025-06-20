#pragma once

#include "AudioDecoder.h"

class MP3AudioDecoder : public AudioDecoder 
{
public:
    MP3AudioDecoder();
    ~MP3AudioDecoder() override;

    bool open(const std::string& filePath) override;
    std::vector<float> decodeNextFrame() override;
    int getSampleRate() const override;
    int getChannels() const override;
    bool isFinished() const override;
    void close() override;

private:
    std::string _filePath;
    int _sampleRate;
    int _channels;
    bool _finished;
};