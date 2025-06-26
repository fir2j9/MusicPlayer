#include "MP3MetadataReader.h"
#include <iostream> // 用于模拟元数据读取
#include <Windows.h> // 引入 Windows 头文件

MP3MetadataReader::MP3MetadataReader() : _filePath(""), _title(""), _artist(""), _album("") {}

MP3MetadataReader::~MP3MetadataReader() {
    close();
}

bool MP3MetadataReader::open(const std::string& filePath) {
    // 模拟读取 MP3 文件的元数据
    std::cout << "Opening MP3 file: " << filePath << std::endl;

    TagLib::FileRef f(filePath.c_str());

    if (f.isNull()) {
        return false;
    }

    TagLib::Tag *tag = f.tag();
    if (tag) {
        // 使用 toUTF8() 方法转换为 UTF-8
        // _title = tag->title().to8Bit(true); // toUTF8(true) 返回 std::string 并释放内部内存
        // _artist = tag->artist().to8Bit(true);
        // _album = tag->album().to8Bit(true);
        _title = tag->title().toCString(true); // toUTF8(true) 返回 std::string 并释放内部内存
        _artist = tag->artist().toCString(true);
        _album = tag->album().toCString(true);
        _filePath = filePath;
        return true;
    }

    return false;
}

std::string MP3MetadataReader::getTitle() const {
    return _title;
}

std::string MP3MetadataReader::getArtist() const {
    return _artist;
}

std::string MP3MetadataReader::getAlbum() const {
    return _album;
}

void MP3MetadataReader::close() {
    std::cout << "Closing MP3 file." << std::endl;
    _filePath = "";
    _title = "";
    _artist = "";
    _album = "";
}