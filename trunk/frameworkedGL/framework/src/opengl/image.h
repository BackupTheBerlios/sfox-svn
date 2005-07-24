#ifndef SE_IMAGE_H
#define SE_IMAGE_H

#include <string>

#include "imageloader.h"

namespace StarEngine {
  class Image {
  protected:
    ImageLoader::ImageData *imgData;

  public:
    Image();
    ~Image();
    void load( const std::string &filename );
    void flipY();
    void flipX();
    ImageLoader::ImageData *getImageData() { return imgData; }
    int getDimension() {
      return (imgData->width!=0)+(imgData->height!=0 )+(imgData->depth!=0);
    }

  };
}

#endif
