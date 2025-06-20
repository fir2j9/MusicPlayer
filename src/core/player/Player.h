#pragma once

#include <string>
#include <iostream> // 用于模拟播放

class Player
{
public:
    Player();
    ~Player();

    bool load(const std::string& filePath);
    bool play();
    bool pause();
    bool stop();
    bool isPlaying() const;

    void setVolume(float volume); // 0.0f - 1.0f
    float getVolume() const;

private:
    std::string _currentFilePath;
    bool _playing;
    float _volume;
};
