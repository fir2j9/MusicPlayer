#include "Player.h"

Player::Player() : _playing(false), _volume(1.0f) {}

Player::~Player() {
    stop(); // 停止播放
}

bool Player::load(const std::string& filePath) {
    // 模拟加载音频文件
    std::cout << "Loading audio file: " << filePath << std::endl;
    _currentFilePath = filePath;
    return true; // 假设加载成功
}

bool Player::play() {
    if (_currentFilePath.empty()) {
        std::cout << "No audio file loaded." << std::endl;
        return false;
    }

    if (_playing) {
        std::cout << "Already playing." << std::endl;
        return true;
    }

    // 模拟播放
    std::cout << "Playing: " << _currentFilePath << std::endl;
    _playing = true;
    return true;
}

bool Player::pause() {
    if (!_playing) {
        std::cout << "Not playing." << std::endl;
        return false;
    }

    // 模拟暂停
    std::cout << "Pausing." << std::endl;
    _playing = false;
    return true;
}

bool Player::stop() {
    if (!_playing && _currentFilePath.empty()) {
        std::cout << "Already stopped." << std::endl;
        return true;
    }

    // 模拟停止
    std::cout << "Stopping." << std::endl;
    _playing = false;
    _currentFilePath = "";
    return true;
}

bool Player::isPlaying() const {
    return _playing;
}

void Player::setVolume(float volume) {
    if (volume >= 0.0f && volume <= 1.0f) {
        _volume = volume;
        std::cout << "Setting volume to: " << volume << std::endl;
    } else {
        std::cout << "Invalid volume value. Volume must be between 0.0 and 1.0." << std::endl;
    }
}

float Player::getVolume() const {
    return _volume;
}
