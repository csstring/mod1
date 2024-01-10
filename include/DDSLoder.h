#pragma once
#include "Common.h"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
class DDSLoder
{
  private:
    const std::string _imagepath;

  public:
    unsigned int format, height, width, linearSize, mipMapCount,fourCC;
    unsigned char* buffer;

  public:
    DDSLoder(const std::string& imagepath) : _imagepath(imagepath), buffer(nullptr){};
    ~DDSLoder()
    {
      if (buffer != nullptr)
        delete []buffer;
    };
    uint32 loadDDS(void);
};