#include "Playlist.h"
#include <fstream>
#include <iostream>
#include <sstream>

Playlist::Playlist() {}

Playlist::~Playlist() {}

void Playlist::addSong(const PlaylistEntry& song) {
    _songs.push_back(song);
}

void Playlist::removeSong(int index) {
    if (index >= 0 && index < _songs.size()) {
        _songs.erase(_songs.begin() + index);
    }
}

// void Playlist::moveSong(int fromIndex, int toIndex) {
//     if (fromIndex >= 0 && fromIndex < _songs.size() && toIndex >= 0 && toIndex < _songs.size()) {
//         std::swap(_songs[fromIndex], _songs[toIndex]);
//     }
// }

void Playlist::moveSong(int fromIndex, int toIndex) {
    if (fromIndex >= 0 && fromIndex < _songs.size() &&
        toIndex >= 0 && toIndex < _songs.size()) {
        PlaylistEntry song = _songs[fromIndex];
        _songs.erase(_songs.begin() + fromIndex);
        _songs.insert(_songs.begin() + toIndex, song);
    }
}

void Playlist::clear() {
    _songs.clear();
}

int Playlist::getSize() const {
    return _songs.size();
}

PlaylistEntry Playlist::getSong(int index) const {
    if (index >= 0 && index < _songs.size()) {
        return _songs[index];
    }
    return PlaylistEntry("", ""); // 返回一个空的 PlaylistEntry
}

bool Playlist::saveToFile(const std::string& filePath) const {
    // std::ofstream file(filePath);
    // if (!file.is_open()) {
    //     return false;
    // }

    // for (const auto& song : _songs) {
    //     file << song._filePath << "\n";
    //     file << song._title << "\n";
    //     file << song._artist << "\n";
    // }

    // file.close();
    // return true;
    return false;
}

bool Playlist::loadFromFile(const std::string& filePath) {
    // std::ifstream file(filePath);
    // if (!file.is_open()) {
    //     return false;
    // }

    // _songs.clear(); // 清空现有播放列表

    // std::string path, title, artist;
    // while (std::getline(file, path)) {
    //     std::getline(file, title);
    //     std::getline(file, artist);
    //     _songs.push_back(PlaylistEntry(path, title, artist));
    // }

    // file.close();
    // return true;
    return false;
}

std::string Playlist::getName() const {
    return _name;
}

void Playlist::setName(const std::string& name) {
    _name = name;
}
