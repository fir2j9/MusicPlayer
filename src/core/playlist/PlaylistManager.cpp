#include "PlaylistManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

#define DEFAULT_PLAYLIST_NAME "default"
#define FAVORITE_PLAYLIST_NAME "like"

PlaylistManager::PlaylistManager() {
    // 创建默认歌单
    createPlaylist(DEFAULT_PLAYLIST_NAME);
    _defaultPlaylistIndex = 0;

    // 创建“我喜欢”歌单
    createPlaylist(FAVORITE_PLAYLIST_NAME);
    _favoritePlaylistIndex = 1;

    _playMode = PlayMode::Sequential;

    _currentPlaylistIndex = _defaultPlaylistIndex;
    _currentSongIndex = -1;

    _regenerateShuffleOrder();
}

PlaylistManager::~PlaylistManager() {}

void PlaylistManager::createPlaylist(const std::string& playlistName) {
    for (const auto& playlist : _playlists) {
        if (playlist.getName() == playlistName) {
            std::cerr << "Playlist already exists: " << playlistName << std::endl;
            return;
        }
    }

    _playlists.emplace_back(); // 创建一个新的 Playlist 对象
    _playlists.back().setName(playlistName);
}

void PlaylistManager::removePlaylist(int index) {
    if (index != _defaultPlaylistIndex && index != _favoritePlaylistIndex &&
        index >= 0 && index < _playlists.size()) {
        _playlists.erase(_playlists.begin() + index);
        // 需要更新 _favoritePlaylistIndex 和 _defaultPlaylistIndex 如果被删除的歌单在它们前面
        if (index < _defaultPlaylistIndex) _defaultPlaylistIndex--;
        if (index < _favoritePlaylistIndex) _favoritePlaylistIndex--;
    } else {
        std::cerr << "Invalid playlist index: " << index << std::endl;
    }
}

void PlaylistManager::renamePlaylist(int index, const std::string& newName) {
    if (index != _defaultPlaylistIndex && index != _favoritePlaylistIndex &&
        index >= 0 && index < _playlists.size()) {
        _playlists[index].setName(newName);
    } else {
        std::cerr << "Invalid playlist index: " << index << std::endl;
    }
}

int PlaylistManager::getPlaylistCount() const {
    return _playlists.size();
}

Playlist& PlaylistManager::getPlaylist(int index) {
    if (index >= 0 && index < _playlists.size()) {
        return _playlists[index];
    }
    static Playlist emptyPlaylist; // 避免返回空指针，返回一个静态的空 Playlist 对象
    return emptyPlaylist;
}

const Playlist& PlaylistManager::getPlaylist(int index) const {
    if (index >= 0 && index < _playlists.size()) {
        return _playlists[index];
    }
    static Playlist emptyPlaylist;
    return emptyPlaylist;
}

int PlaylistManager::getDefaultPlaylistIndex() const {
    return _defaultPlaylistIndex;
}

int PlaylistManager::getFavoritePlaylistIndex() const {
    return _favoritePlaylistIndex;
}

void PlaylistManager::addSongToPlaylist(int playlistIndex, const PlaylistEntry& song) {
    if (playlistIndex >= 0 && playlistIndex < _playlists.size()) {
        _playlists[playlistIndex].addSong(song);
    }
}

void PlaylistManager::removeSongFromPlaylist(int playlistIndex, int songIndex) {
    if (playlistIndex >= 0 && playlistIndex < _playlists.size()) {
        _playlists[playlistIndex].removeSong(songIndex);
    }
}

void PlaylistManager::moveSongInPlaylist(int playlistIndex, int fromIndex, int toIndex) {
    if (playlistIndex >= 0 && playlistIndex < _playlists.size()) {
        _playlists[playlistIndex].moveSong(fromIndex, toIndex);
    }
}

void PlaylistManager::clearPlaylist(int playlistIndex) {
    if (playlistIndex >= 0 && playlistIndex < _playlists.size()) {
        _playlists[playlistIndex].clear();
    }
}

bool PlaylistManager::savePlaylistsToFile(const std::string& filePath) const {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for saving: " << filePath << std::endl;
        return false;
    }

    // 保存歌单数量
    file << _playlists.size() << std::endl;

    // 保存每个歌单
    for (const auto& playlist : _playlists) {
        file << playlist.getName() << std::endl; // 保存歌单名字
        file << playlist.getSize() << std::endl; // 保存歌曲数量
        for (int i = 0; i < playlist.getSize(); ++i) {
            const PlaylistEntry& song = playlist.getSong(i);
            file << song._filePath << std::endl;
            file << song._title << std::endl;
            file << song._artist << std::endl;
        }
    }

    file.close();
    std::cout << "Playlists saved to: " << filePath << std::endl;
    return true;
}

bool PlaylistManager::loadPlaylistsFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for loading: " << filePath << std::endl;
        return false;
    }

    _playlists.clear(); // 清空现有歌单
    int playlistCount;
    file >> playlistCount;
    file.ignore(); // 忽略换行符

    for (int i = 0; i < playlistCount; ++i) {
        std::string playlistName;
        std::getline(file, playlistName);

        createPlaylist(playlistName); // 创建歌单

        int songCount;
        file >> songCount;
        file.ignore();

        for (int j = 0; j < songCount; ++j) {
            PlaylistEntry song;
            std::string line;

            std::getline(file, song._filePath);
            std::getline(file, song._title);
            std::getline(file, song._artist);

            addSongToPlaylist(i, song);
        }
    }
    _defaultPlaylistIndex = _findPlaylistIndex(DEFAULT_PLAYLIST_NAME);
    _favoritePlaylistIndex = _findPlaylistIndex(FAVORITE_PLAYLIST_NAME);
    file.close();
    std::cout << "Playlists loaded from: " << filePath << std::endl;
    return true;
}

void PlaylistManager::setPlayMode(PlayMode playMode) {
    if (_playMode != playMode) {
        _playMode = playMode;
        if (playMode == PlayMode::Shuffle) {
            _regenerateShuffleOrder();
        }
    }
}

int PlaylistManager::_findPlaylistIndex(const std::string& playlistName) const {
    for (size_t i = 0; i < _playlists.size(); ++i) {
        if (_playlists[i].getName() == playlistName) {
            return static_cast<int>(i);
        }
    }
    return -1; // 未找到
}

void PlaylistManager::_regenerateShuffleOrder() {
    _shuffleOrder.clear();
    for (int i = 0; i < _playlists[_currentPlaylistIndex].getSize(); i++) {
        _shuffleOrder.push_back(i);
    }
    
    // 生成随机播放顺序
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(_shuffleOrder.begin(), _shuffleOrder.end(), g);
    
    _shuffleIndex = 0; // 重置随机索引
}

int PlaylistManager::getNextIndex() {
    if (_playlists[_currentPlaylistIndex].getSize() == 0) return -1;
    
    switch (_playMode) {
        case PlayMode::SingleLoop:
        case PlayMode::Sequential:
            if (_currentSongIndex + 1 < _playlists[_currentPlaylistIndex].getSize()) {
                _currentSongIndex++;
            } else {
                _currentSongIndex = 0;
            }
            return _currentSongIndex;
            
        case PlayMode::Shuffle:
            if (_shuffleOrder.empty()) return 0;
            _shuffleIndex = (_shuffleIndex + 1) % _shuffleOrder.size();
            _currentSongIndex = _shuffleOrder[_shuffleIndex];
            return _currentSongIndex;

        default:
            return 0;
    }
}

int PlaylistManager::getPreviousIndex() {
    if (_playlists[_currentPlaylistIndex].getSize() == 0) return -1;
    
    switch (_playMode) {
        case PlayMode::SingleLoop:
        case PlayMode::Sequential:
            if (_currentSongIndex > 0) {
                _currentSongIndex--;
            } else {
                _currentSongIndex = _playlists[_currentPlaylistIndex].getSize() - 1;
            }
            return _currentSongIndex;
            
        case PlayMode::Shuffle:
            if (_shuffleOrder.empty()) return 0;
            _shuffleIndex = _shuffleIndex == 0 ? _shuffleOrder.size() - 1 : _shuffleIndex - 1;
            _currentSongIndex = _shuffleOrder[_shuffleIndex];
            return _currentSongIndex;

        default:
            return 0;
    }
}

int PlaylistManager::autoplayNextIndex() {
    if (_playlists[_currentPlaylistIndex].getSize() == 0) return -1;
    
    switch (_playMode) {
        case PlayMode::Sequential:
            if (_currentSongIndex + 1 < _playlists[_currentPlaylistIndex].getSize()) {
                _currentSongIndex++;
            } else {
                _currentSongIndex = 0;
            }
            return _currentSongIndex;
            
        case PlayMode::Shuffle:
            if (_shuffleOrder.empty()) return 0;
            _shuffleIndex = (_shuffleIndex + 1) % _shuffleOrder.size();
            _currentSongIndex = _shuffleOrder[_shuffleIndex];
            return _currentSongIndex;
            
        case PlayMode::SingleLoop:
            return _currentSongIndex; // 单曲循环返回当前歌曲
            
        default:
            return 0;
    }
}
