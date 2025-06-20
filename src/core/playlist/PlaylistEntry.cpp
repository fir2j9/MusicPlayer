#include "PlaylistEntry.h"

std::string PlaylistEntry::getDisplayName() const {
    std::string displayName;
    if (_title.empty()) {
        // 如果歌曲标题为空，则使用文件名作为显示名称
        size_t pos = _filePath.find_last_of("\\");
        if (pos == std::string::npos) {
            pos = _filePath.find_last_of("/");
        }
        if (pos != std::string::npos) {
            displayName = _filePath.substr(pos + 1);
        } else {
            displayName = _filePath;
        }
    } else {
        displayName =  _artist + " - " + _title;
    }
    
    return displayName;
}
