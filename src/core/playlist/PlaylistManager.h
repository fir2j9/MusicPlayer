#pragma once

#include <vector>
#include <string>
#include <random>
#include "Playlist.h"

class PlaylistManager {
public:
     // 添加枚举类型
    enum class PlayMode {
        Sequential,  // 顺序播放（列表模式）
        Shuffle,     // 随机播放
        SingleLoop,   // 单曲循环
        ModeMax
    };

    PlaylistManager();
    ~PlaylistManager();

    // 歌单管理
    void createPlaylist(const std::string& playlistName);
    void removePlaylist(int index);  // 移除歌单
    void renamePlaylist(int index, const std::string& newName);

    int getPlaylistCount() const;
    Playlist& getPlaylist(int index); // 获取可修改的 Playlist 引用
    const Playlist& getPlaylist(int index) const; // 获取只读 Playlist 引用
    int getDefaultPlaylistIndex() const;
    int getFavoritePlaylistIndex() const;

    // 歌曲管理（委托给当前选中的歌单）
    void addSongToPlaylist(int playlistIndex, const PlaylistEntry& song); // 添加歌曲到指定歌单
    void removeSongFromPlaylist(int playlistIndex, int songIndex); // 从指定歌单移除歌曲
    void moveSongInPlaylist(int playlistIndex, int fromIndex, int toIndex); // 在指定歌单中移动歌曲
    void clearPlaylist(int playlistIndex); // 清空指定歌单

    // 保存和加载歌单数据
    bool savePlaylistsToFile(const std::string& filePath) const;
    bool loadPlaylistsFromFile(const std::string& filePath);

    PlayMode getPlayMode() const { return _playMode; }
    void setPlayMode(PlayMode playMode);

     // 根据播放模式获取下一首歌曲
    int getNextIndex();
    int getPreviousIndex();
    int autoplayNextIndex(); //     自动播放下一首歌曲

    int getCurrentPlaylistIndex() const { return _currentPlaylistIndex; }
    int getCurrentSongIndex() const { return _currentSongIndex; }

    void setCurrentPlaylistIndex(int index) { _currentPlaylistIndex = index; }
    void setCurrentSongIndex(int index) { _currentSongIndex = index; }

private:
    std::vector<Playlist> _playlists;
    int _defaultPlaylistIndex;
    int _favoritePlaylistIndex;
    PlayMode _playMode;

    int _currentPlaylistIndex; // 当前选择的歌单索引
    int _currentSongIndex; // 当前播放的歌曲索引

    int _shuffleIndex = 0;       // 当前随机播放索引
    std::vector<int> _shuffleOrder; // 用于存储随机播放顺序

    // 辅助函数
    int _findPlaylistIndex(const std::string& playlistName) const;
    // 重新生成随机顺序
    void _regenerateShuffleOrder();
};