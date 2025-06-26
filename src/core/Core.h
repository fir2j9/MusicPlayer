#pragma once

#include "Player.h"
#include "PlaylistManager.h"
#include "MetadataReader.h"
#include "AudioDecoder.h"

class Core {
public:
    Core();
    ~Core();

    // 播放控制
    bool play();
    bool pause();
    bool stop();
    bool playSong(int playlistIndex, int songIndex); // 播放指定索引的歌曲

    bool isPlaying() const;

    void setPlayMode(PlaylistManager::PlayMode mode); // 设置播放模式
    PlaylistManager::PlayMode getPlayMode() const; // 获取播放模式

    int next();       // 下一首（根据播放模式）
    int previous();   // 上一首（根据播放模式）
    int autoPlay();    // 自动播放下一首

    int getCurrentPlaylistIndex() const; // 获取当前播放列表索引
    int getCurrentSongIndex() const;      // 获取当前歌曲索引

    void setCurrentPlaylistIndex(int index); // 设置当前播放列表索引
    void setCurrentSongIndex(int index);      // 设置当前歌曲索引

    double getCurrentPosition() const; // 获取当前播放位置
    double getTotalDuration() const;    // 获取总时长
    int getPlaybackProgress() const; // 获取播放进度(0-100)

    // 播放列表管理
    void addSong(int playlistIndex, const std::string& filePath);
    void removeSong(int playlistIndex, int songIndex);
    void moveSong(int playlistIndex, int fromIndex, int toIndex);
    void clearPlaylist(int playlistIndex);

    int getPlaylistCount() const;
    Playlist& getPlaylist(int index);
    const Playlist& getPlaylist(int index) const;
    int getDefaultPlaylistIndex() const;
    int getFavoritePlaylistIndex() const;

    // 歌单管理
    void createPlaylist(const std::string& playlistName);
    void removePlaylist(int index);  // 移除歌单
    void renamePlaylist(int index, const std::string& newName);

    int getPlaylistSize(int playlistIndex) const; // 获取指定歌单的歌曲数量
    PlaylistEntry getSong(int playlistIndex, int songIndex) const; // 获取指定歌单的歌曲

    // 音量控制
    void setVolume(float volume);
    float getVolume() const;

    // 保存和加载播放列表
    bool savePlaylistsToFile(const std::string& filePath) const;
    bool loadPlaylistsFromFile(const std::string& filePath);

private:
    Player _player;
    PlaylistManager _playlistManager;
    MetadataReader* _metadataReader; // 根据文件类型选择不同的 MetadataReader
    AudioDecoder* _audioDecoder;     // 根据文件类型选择不同的 AudioDecoder

    // 辅助函数
    bool _loadSong(const std::string& filePath); // 加载歌曲
};