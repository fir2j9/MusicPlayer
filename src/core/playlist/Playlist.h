#pragma once

#include <vector>
#include <string>
#include "PlaylistEntry.h"

class Playlist
{
public:
    Playlist();
    ~Playlist();

    void addSong(const PlaylistEntry& song);
    void removeSong(int index);
    void moveSong(int fromIndex, int toIndex);
    void clear();

    int getSize() const;
    PlaylistEntry getSong(int index) const;

    // 保存和加载播放列表
    bool saveToFile(const std::string& filePath) const; //不再需要
    bool loadFromFile(const std::string& filePath); //不再需要
    std::string getName() const;
    void setName(const std::string& name);

private:
    std::vector<PlaylistEntry> _songs;
    std::string _name; //歌单名字
};
