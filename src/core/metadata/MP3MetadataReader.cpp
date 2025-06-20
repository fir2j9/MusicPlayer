#include "MP3MetadataReader.h"
#include <iostream> // 用于模拟元数据读取

MP3MetadataReader::MP3MetadataReader() : _filePath(""), _title(""), _artist(""), _album("") {}

MP3MetadataReader::~MP3MetadataReader() {
    close();
}

bool MP3MetadataReader::open(const std::string& filePath) {
    // 模拟读取 MP3 文件的元数据
    std::cout << "Opening MP3 file: " << filePath << std::endl;
    _filePath = filePath;

    // 模拟元数据
    _title = "Unknown Title";
    _artist = "Unknown Artist";
    _album = "Unknown Album";

    return true; // 假设读取成功
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
