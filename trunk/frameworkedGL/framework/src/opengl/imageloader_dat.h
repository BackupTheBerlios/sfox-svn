#ifndef SE_IMAGE_LOADER_DAT_H
#define SE_IMAGE_LOADER_DAT_H

#include "opengl/pixelformat.h"
#include "opengl/imageloader.h"

namespace StarEngine {
  class ImageLoaderDat : public ImageLoader {
  public:
    ImageData *load( const std::string &filename );
    static bool supportType( const std::string &lowerCase );
  private:
    static const char *supportedType[1];
  };
}

#endif
