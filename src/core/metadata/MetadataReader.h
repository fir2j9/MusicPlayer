#pragma once

#include <string>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <memory>

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