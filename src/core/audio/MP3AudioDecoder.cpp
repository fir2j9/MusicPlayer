#include "MP3AudioDecoder.h"
#include <iostream> // 用于模拟音频解码
#include <vector>

MP3AudioDecoder::MP3AudioDecoder() : _filePath(""), _sampleRate(44100), _channels(2), _finished(false) {}

MP3AudioDecoder::~MP3AudioDecoder() {
    close();
}

bool MP3AudioDecoder::open(const std::string& filePath) {
    // 模拟打开 MP3 文件
    std::cout << "Opening MP3 file: " << filePath << std::endl;
    _filePath = filePath;
    _finished = false;
    return true; // 假设打开成功
}

std::vector<float> MP3AudioDecoder::decodeNextFrame() {
    // 模拟解码 MP3 文件的一帧
    std::cout << "Decoding next frame." << std::endl;

    // 模拟音频数据
    std::vector<float> frameData;
    for (int i = 0; i < 1024; ++i) {
        frameData.push_back(sin(i * 0.1f)); // 模拟正弦波
    }

    _finished = true; // 模拟文件已解码完毕
    return frameData;
}

int MP3AudioDecoder::getSampleRate() const {
    return _sampleRate;
}

int MP3AudioDecoder::getChannels() const {
    return _channels;
}

bool MP3AudioDecoder::isFinished() const {
    return _finished;
}

void MP3AudioDecoder::close() {
    std::cout << "Closing MP3 file." << std::endl;
    _filePath = "";
    _finished = true;
}
