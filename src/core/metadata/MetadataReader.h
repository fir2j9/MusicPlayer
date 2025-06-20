#pragma once

#include <string>

class MetadataReader
{
public:
    virtual ~MetadataReader() {}

    virtual bool open(const std::string& filePath) = 0;
    virtual std::string getTitle() const = 0;
    virtual std::string getArtist() const = 0;
    virtual std::string getAlbum() const = 0;

    virtual void close() = 0;
};