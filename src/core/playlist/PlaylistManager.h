#pragma once

#include <vector>
#include <string>
#include "Playlist.h"

class PlaylistManager {
public:
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

private:
    std::vector<Playlist> _playlists;
    int _defaultPlaylistIndex;
    int _favoritePlaylistIndex;

    // 辅助函数
    int _findPlaylistIndex(const std::string& playlistName) const;
};