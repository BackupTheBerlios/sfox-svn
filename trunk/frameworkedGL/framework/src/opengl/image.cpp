#include <cassert>
#include "opengl/image.h"
#include "framework/exception.h"

namespace StarEngine {
  Image::Image()
    :imgData( 0 )
  {
  }

  Image::~Image()
  {
    if ( imgData ) {
      switch( imgData->pixelFormat ) {
      case PF_RGB:
      case PF_RGBA:
      case PF_ALPHA8:
        delete[] ( unsigned char * )imgData->data;
        break;
      default:
        assert( 0 );
        break;
      }
      delete imgData;
      imgData = 0;
    }
  }
  void
  Image::load( const std::string &filename )
  {
    using namespace std;
    size_t pos = filename.find_last_of('.');
    if ( pos == string::npos ) {
      std::string mess("Image::load: Can't load image ");
      mess.append(filename);
      throw new Exception(mess);
    }
    const string ext = filename.substr(pos+1);
    ImageLoader *imgLoader = ImageLoader::getImageLoader(ext);
    imgData = imgLoader->load(filename);
  }

  void
  Image::flipY()
  {
    assert( imgData );
    switch( imgData->pixelFormat ) {
    case PF_RGB:
    case PF_RGBA:
    case PF_ALPHA8: {
      int bpp = PixelFormatUtils::getBytesPerPixel( imgData->pixelFormat );
      unsigned char *src = (unsigned char *)imgData->data;
      unsigned char *tmpBuffer = new unsigned char[imgData->width*imgData->height*bpp];
      int offsetSrc = 0;
      int offsetDst = (imgData->height-1)*imgData->width*bpp;
      for ( int j = 0; j < imgData->height; j++ ) {
        memcpy( tmpBuffer+offsetDst, src+offsetSrc, imgData->width*bpp );
        offsetSrc += imgData->width*bpp;
        offsetDst -= imgData->width*bpp;
      }
      imgData->data = tmpBuffer;
      delete[] src;
      break;
    }
    default:
      assert( 0 );
      break;
    }
  }

  void
  Image::flipX()
  {
    assert( imgData );
    switch( imgData->pixelFormat ) {
    case PF_RGB:
    case PF_RGBA:
    case PF_ALPHA8: {
      int bpp = PixelFormatUtils::getBytesPerPixel( imgData->pixelFormat );
      unsigned char *src = (unsigned char *)imgData->data;
      unsigned char *tmpBuffer = new unsigned char[imgData->width*imgData->height*bpp];
      for ( int j = 0; j < imgData->height; j++ )
        for ( int i = 0; i < imgData->width*bpp; i+=bpp )
          for ( int c = 0; c < bpp; c++ )
            tmpBuffer[(imgData->width-1)*bpp-i+c+j*imgData->width*bpp] = src[i+j*imgData->width*bpp+c];
      imgData->data = tmpBuffer;
      delete[] src;
      break;
    }
    default:
      assert( 0 );
      break;
    }
  }
};
