#pragma once

#include "MetadataReader.h"

class MP3MetadataReader : public MetadataReader 
{
public:
    MP3MetadataReader();
    ~MP3MetadataReader() override;

    bool open(const std::string& filePath) override;
    std::string getTitle() const override;
    std::string getArtist() const override;
    std::string getAlbum() const override;

    void close() override;

private:
    std::string _filePath;
    std::string _title;
    std::string _artist;
    std::string _album;
};
