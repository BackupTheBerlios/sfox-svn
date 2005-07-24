#ifndef SE_IMAGE_LOADER_H
#define SE_IMAGE_LOADER_H

#include "pixelformat.h"

namespace StarEngine {
  class ImageLoader {
  public:
    struct ImageData {
      void *data;
      int width;
      int height;
      int depth;
      PixelFormat pixelFormat;
    };
    static ImageLoader *getImageLoader( const std::string &ext );
    virtual ImageData *load( const std::string &filename ) = 0;
  };

  class ImageLoaderSDL_Image : public ImageLoader {
  public:
    ImageData *load( const std::string &filename );
    static bool supportType( const std::string &lowerCase );
  private:
    static const char *supportedType[4];
  };
}

#endif
