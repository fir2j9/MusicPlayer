#pragma once

#include <string>
#include <iostream>
#include <mutex>
#include <atomic>
#include <portaudio.h>
#include <sndfile.h>
#include <cmath> // 用于音量计算

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

    double getTotalDuration() const;
    double getCurrentPlaybackTime() const;
    int getPlaybackProgress() const; // 0 - 100

private:
    enum class State {
        Stopped,
        Playing,
        Paused
    };

    // PortAudio 回调函数
    static int paCallback(const void* input, void* output,
                          unsigned long frameCount,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void* userData);
    
    // 重置播放器状态
    void resetPlayback();
    
    // 成员变量
    std::string _currentFilePath;
    std::atomic<State> _state;
    std::atomic<float> _volume;
    
    // PortAudio 相关
    PaStream* _stream;
    PaStreamParameters _outputParameters;
    
    // Libsndfile 相关
    SNDFILE* _sndFile;
    SF_INFO _sndInfo;
    
    // 播放状态
    std::atomic<sf_count_t> _currentFrame;    // 当前帧位置
    std::atomic<double> _currentPlaybackTime; // 当前播放时间
    std::atomic<double> _totalDuration;       // 总时长
    bool _streamReady;                        // 流是否已准备好
    
    // 线程安全
    mutable std::mutex _mutex;
};
