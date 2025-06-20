#pragma once

#include <string>
#include <vector>

class AudioDecoder 
{
public:
    virtual ~AudioDecoder() {}

    virtual bool open(const std::string& filePath) = 0;
    virtual std::vector<float> decodeNextFrame() = 0; // 返回解码后的音频数据
    virtual int getSampleRate() const = 0;
    virtual int getChannels() const = 0;
    virtual bool isFinished() const = 0;
    virtual void close() = 0;
};
