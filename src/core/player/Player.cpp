#include "Player.h"
#include <thread>
#include <iomanip> // 用于时间格式化

Player::Player()
    : _state(State::Stopped),
      _volume(1.0f),
      _stream(nullptr),
      _sndFile(nullptr),
      _currentFrame(0),
      _currentPlaybackTime(0.0),
      _totalDuration(0.0),
      _streamReady(false)
{
    std::memset(&_sndInfo, 0, sizeof(_sndInfo));
    
    // 初始化 PortAudio
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio initialization error: " << Pa_GetErrorText(err) << std::endl;
    }
}

Player::~Player()
{
    stop();
    
    if (_sndFile) {
        sf_close(_sndFile);
        _sndFile = nullptr;
    }
    
    if (_streamReady) {
        Pa_CloseStream(_stream);
    }
    
    Pa_Terminate();
}

bool Player::load(const std::string& filePath)
{
    // 加锁确保线程安全
    std::lock_guard<std::mutex> lock(_mutex);
    
    // 如果已经有文件打开，先关闭
    if (_sndFile) {
        sf_close(_sndFile);
        _sndFile = nullptr;
    }
    
    // 打开新文件
    std::memset(&_sndInfo, 0, sizeof(_sndInfo));
    _sndFile = sf_open(filePath.c_str(), SFM_READ, &_sndInfo);
    
    if (!_sndFile) {
        std::cerr << "Error opening file: " << filePath 
                  << "\nReason: " << sf_strerror(_sndFile) << std::endl;
        return false;
    }
    
    _currentFilePath = filePath;
    _totalDuration = static_cast<double>(_sndInfo.frames) / _sndInfo.samplerate;
    resetPlayback();
    _state = State::Stopped;
    
    std::cout << "Successfully loaded: " << filePath 
              << "\n  Duration: " << std::setprecision(2) 
              << _totalDuration << " seconds"
              << "\n  Sample rate: " << _sndInfo.samplerate
              << "\n  Channels: " << _sndInfo.channels
              << "\n  Format: " << _sndInfo.format << std::endl;
    
    return true;
}

void Player::resetPlayback()
{
    _currentFrame = 0;
    _currentPlaybackTime = 0.0;
}

bool Player::play()
{
    std::lock_guard<std::mutex> lock(_mutex);
    
    // 检查是否已加载文件
    if (!_sndFile || _currentFilePath.empty()) {
        std::cerr << "No file loaded for playback." << std::endl;
        return false;
    }
    
    // 如果已经在播放，则直接返回
    if (_state == State::Playing) {
        std::cout << "Playback is already in progress." << std::endl;
        return true;
    }
    
    // 准备并启动流
    if (_state == State::Paused || _state == State::Stopped) {
        PaError err;
        
        // 如果是停止状态，重新配置输出参数
        if (_state == State::Stopped) {
            int outputDevice = Pa_GetDefaultOutputDevice();
            if (outputDevice == paNoDevice) {
                std::cerr << "No default output device available." << std::endl;
                return false;
            }
            
            const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(outputDevice);
            std::cout << "Using audio device: " << deviceInfo->name << std::endl;
            
            _outputParameters.device = outputDevice;
            _outputParameters.channelCount = _sndInfo.channels;
            _outputParameters.sampleFormat = paFloat32; // 32位浮点格式
            _outputParameters.suggestedLatency = deviceInfo->defaultLowOutputLatency;
            _outputParameters.hostApiSpecificStreamInfo = nullptr;
            
            // 重置文件位置
            resetPlayback();
            sf_seek(_sndFile, 0, SEEK_SET);
        }
        
        // 如果尚未创建流，则打开它
        if (!_streamReady) {
            err = Pa_OpenStream(&_stream,
                                nullptr, // 无输入
                                &_outputParameters,
                                static_cast<double>(_sndInfo.samplerate),
                                paFramesPerBufferUnspecified, // 使用默认缓冲
                                paClipOff, // 关闭剪切处理
                                paCallback,
                                this); // 传递this指针
            
            if (err != paNoError) {
                std::cerr << "PortAudio stream open error: " << Pa_GetErrorText(err) << std::endl;
                return false;
            }
            
            _streamReady = true;
        }
        
        // 开始播放
        err = Pa_StartStream(_stream);
        if (err != paNoError) {
            std::cerr << "PortAudio stream start error: " << Pa_GetErrorText(err) << std::endl;
            return false;
        }
        
        _state = State::Playing;
        std::cout << "Playback started: " << _currentFilePath << std::endl;
        return true;
    }
    
    return false;
}

bool Player::pause()
{
    std::lock_guard<std::mutex> lock(_mutex);
    
    if (_state == State::Playing && _streamReady) {
        PaError err = Pa_StopStream(_stream);
        if (err != paNoError) {
            std::cerr << "PortAudio stream stop error: " << Pa_GetErrorText(err) << std::endl;
            return false;
        }
        
        _state = State::Paused;
        std::cout << "Playback paused." << std::endl;
        return true;
    }
    
    return false;
}

bool Player::stop()
{
    std::lock_guard<std::mutex> lock(_mutex);
    
    if (_state != State::Stopped && _streamReady) {
        PaError err = Pa_StopStream(_stream);
        if (err != paNoError && err != paStreamIsStopped) {
            std::cerr << "PortAudio stream stop error: " << Pa_GetErrorText(err) << std::endl;
            return false;
        }
        
        // 仅在状态为Stopped时关闭流
        if (_state != State::Stopped) {
            Pa_CloseStream(_stream);
            _streamReady = false;
            resetPlayback();
            _state = State::Stopped;
            std::cout << "Playback stopped." << std::endl;
        }
        return true;
    }
    
    return false;
}

bool Player::isPlaying() const
{
    return _state == State::Playing;
}

void Player::setVolume(float volume)
{
    // 限制音量在0.0 - 1.0之间
    volume = volume < 0.0f ? 0.0f : (volume > 1.0f ? 1.0f : volume);
    _volume = volume;
}

float Player::getVolume() const
{
    return _volume;
}

double Player::getTotalDuration() const
{
    return _totalDuration;
}

double Player::getCurrentPlaybackTime() const
{
    return _currentPlaybackTime;
}

int Player::getPlaybackProgress() const
{
    if (_totalDuration <= 0.0) return 0;
    return static_cast<int>((_currentPlaybackTime / _totalDuration) * 100.0);
}

// PortAudio 回调函数
int Player::paCallback(const void* /* input */, void* output,
                       unsigned long frameCount,
                       const PaStreamCallbackTimeInfo* /* timeInfo */,
                       PaStreamCallbackFlags /* statusFlags */,
                       void* userData)
{
    Player* player = static_cast<Player*>(userData);
    float* out = static_cast<float*>(output);
    
    // 如果播放器处于暂停或停止状态
    if (player->_state != Player::State::Playing || player->_sndFile == nullptr) {
        // 填充静音
        std::memset(out, 0, frameCount * player->_sndInfo.channels * sizeof(float));
        return paContinue;
    }
    
    // 从文件中读取音频数据
    sf_count_t framesRead = sf_readf_float(player->_sndFile, out, static_cast<sf_count_t>(frameCount));
    
    // 应用音量控制
    float volume = player->_volume;
    if (volume != 1.0f) {
        for (sf_count_t i = 0; i < framesRead * player->_sndInfo.channels; ++i) {
            out[i] *= volume;
        }
    }
    
    // 如果读取的帧数少于请求的帧数，表示已到文件末尾
    if (framesRead < static_cast<sf_count_t>(frameCount)) {
        // 填充剩余部分为静音
        std::memset(out + framesRead * player->_sndInfo.channels, 0,
                   (frameCount - framesRead) * player->_sndInfo.channels * sizeof(float));
        
        // 通知主线程播放已完成
        player->_currentFrame += framesRead;
        player->_currentPlaybackTime = static_cast<double>(player->_currentFrame) / player->_sndInfo.samplerate;
        
        // 如果文件结束
        if (framesRead == 0) {
            player->_state = Player::State::Stopped;
            return paComplete;
        }
    }
    
    // 更新播放位置
    player->_currentFrame += framesRead;
    player->_currentPlaybackTime = static_cast<double>(player->_currentFrame) / player->_sndInfo.samplerate;
    
    return paContinue;
}
