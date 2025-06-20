#pragma once
#include <string>

class PlaylistEntry 
{
public:
    std::string _filePath;  // 歌曲文件路径
    std::string _title;     // 歌曲标题
    std::string _artist;    // 歌曲艺术家

    PlaylistEntry() {};
    PlaylistEntry(const std::string& path, const std::string& title = "", const std::string& artist = "")
        : _filePath(path), _title(title), _artist(artist) {}

    ~PlaylistEntry() {}

    // 获取列表展示的名称(艺术家-标题)
    std::string getDisplayName() const;
private:

};