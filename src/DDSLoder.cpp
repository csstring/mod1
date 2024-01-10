#include "DDSLoder.h"
#include "GL/glew.h"

uint32 DDSLoder::loadDDS(void)
{
  uint8 header[124];

  FILE *fp;

  fp = fopen(_imagepath.c_str(), "rb");
  if (fp == NULL)
    return 0;

    /* 파일 타입 체크 */
  char filecode[4];
  fread(filecode, 1, 4, fp);
  if (strncmp(filecode, "DDS ", 4) != 0) {
      fclose(fp);
      return 0;
  }

  fread(&header, 124, 1, fp);

  height = *(uint32*)&(header[8]);
  width = *(uint32*)&(header[12]);
  linearSize = *(uint32*)&(header[16]);
  mipMapCount = *(uint32*)&(header[24]);
  fourCC = *(uint32*)&(header[80]);

  uint32 bufsize;

  bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
  buffer = new uint8[bufsize*sizeof(uint8)];
  fread(buffer, 1, bufsize, fp);

  fclose(fp);

  switch(fourCC)
  {
  case FOURCC_DXT1:
      format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
      break;
  case FOURCC_DXT3:
      format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
      break;
  case FOURCC_DXT5:
      format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
      break;
  default:
      return 0;
  }

  uint32 textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  uint32 blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
  uint32 offset = 0;

  for (uint32 level = 0; level < mipMapCount && (width || height); ++level)
  {
      uint32 size = ((width+3)/4)*((height+3)/4)*blockSize;
      glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
            0, size, buffer + offset);

      offset += size;
      width  /= 2;
      height /= 2;
  }

  return textureID;
}