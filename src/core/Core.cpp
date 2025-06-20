#include "Core.h"
#include "MP3MetadataReader.h" // 需要包含具体的 MetadataReader 实现
#include "MP3AudioDecoder.h"   // 需要包含具体的 AudioDecoder 实现
#include "Utils.h" // 如果需要使用 Utils

Core::Core() : _metadataReader(nullptr), _audioDecoder(nullptr) {}

Core::~Core() {
    if (_metadataReader) delete _metadataReader;
    if (_audioDecoder) delete _audioDecoder;
}

bool Core::play() {
    return _player.play();
}

bool Core::pause() {
    return _player.pause();
}

bool Core::stop() {
    return _player.stop();
}

bool Core::playSong(int playlistIndex, int songIndex) {
    if (playlistIndex >= 0 && playlistIndex < _playlistManager.getPlaylistCount() &&
        songIndex >= 0 && songIndex < _playlistManager.getPlaylist(playlistIndex).getSize()) {
        PlaylistEntry song = _playlistManager.getPlaylist(playlistIndex).getSong(songIndex);
        return _loadSong(song._filePath) && _player.play();
    }
    return false;
}

bool Core::isPlaying() const {
    return _player.isPlaying();
}

void Core::addSong(int playlistIndex, const std::string& filePath) {
    // 使用 MetadataReader 获取歌曲元数据
    MetadataReader* metadataReader = new MP3MetadataReader(); // 根据文件类型选择不同的 MetadataReader

    if (metadataReader->open(filePath)) {
        std::string title = metadataReader->getTitle();
        std::string artist = metadataReader->getArtist();
        _playlistManager.addSongToPlaylist(playlistIndex, PlaylistEntry(filePath, title, artist));
        metadataReader->close();
    } else {
        // 如果无法读取元数据，则使用文件名
        std::string filename = filePath.substr(filePath.find_last_of("/\\") + 1);
        _playlistManager.addSongToPlaylist(playlistIndex, PlaylistEntry(filePath, filename, "Unknown Artist"));
    }
    delete metadataReader;
}

void Core::removeSong(int playlistIndex, int songIndex) {
    _playlistManager.removeSongFromPlaylist(playlistIndex, songIndex);
}

void Core::moveSong(int playlistIndex, int fromIndex, int toIndex) {
    _playlistManager.moveSongInPlaylist(playlistIndex, fromIndex, toIndex);
}

void Core::clearPlaylist(int playlistIndex) {
    _playlistManager.clearPlaylist(playlistIndex);
}

int Core::getPlaylistCount() const {
    return _playlistManager.getPlaylistCount();
}

Playlist& Core::getPlaylist(int index) {
    return _playlistManager.getPlaylist(index);
}

const Playlist& Core::getPlaylist(int index) const {
    return _playlistManager.getPlaylist(index);
}

int Core::getDefaultPlaylistIndex() const {
    return _playlistManager.getDefaultPlaylistIndex();
}

int Core::getFavoritePlaylistIndex() const {
    return _playlistManager.getFavoritePlaylistIndex();
}

int Core::getPlaylistSize(int playlistIndex) const {
    return _playlistManager.getPlaylist(playlistIndex).getSize();
}

PlaylistEntry Core::getSong(int playlistIndex, int songIndex) const {
    return _playlistManager.getPlaylist(playlistIndex).getSong(songIndex);
}

void Core::setVolume(float volume) {
    _player.setVolume(volume);
}

float Core::getVolume() const {
    return _player.getVolume();
}

bool Core::_loadSong(const std::string& filePath) {
    // 1. 停止当前播放
    _player.stop();

    // 2. 释放之前的 MetadataReader 和 AudioDecoder
    if (_metadataReader) delete _metadataReader;
    if (_audioDecoder) delete _audioDecoder;

    // 3. 根据文件类型选择 MetadataReader 和 AudioDecoder
    _metadataReader = new MP3MetadataReader(); // 根据文件类型选择不同的 MetadataReader
    _audioDecoder = new MP3AudioDecoder();   // 根据文件类型选择不同的 AudioDecoder

    // 4. 打开文件
    if (!_metadataReader->open(filePath) || !_audioDecoder->open(filePath)) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    // 5. 加载文件到 Player
    return _player.load(filePath);
}

bool Core::savePlaylistsToFile(const std::string& filePath) const {
    return _playlistManager.savePlaylistsToFile(filePath);
}

bool Core::loadPlaylistsFromFile(const std::string& filePath) {
    return _playlistManager.loadPlaylistsFromFile(filePath);
}

void Core::createPlaylist(const std::string& playlistName) {
    _playlistManager.createPlaylist(playlistName);
}

void Core::removePlaylist(int index) {
    _playlistManager.removePlaylist(index);
}

void Core::renamePlaylist(int index, const std::string& newName) {
    _playlistManager.renamePlaylist(index, newName);
}