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
    void setImageData(ImageLoader::ImageData *img) { imgData = img; }
    int getDimension() {
      return (imgData->width>1)+(imgData->height>1)+(imgData->depth>1);
    }

  };
}

#endif
