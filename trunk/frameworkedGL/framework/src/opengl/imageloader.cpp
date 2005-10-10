#ifdef WIN32
#include <windows.h>
#endif

#include <cassert>
#include <ctype.h>
#include <string>
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>

#include "opengl/imageloader.h"
#include "opengl/imageloader_dat.h"
#include "framework/exception.h"

namespace StarEngine {
  const char *ImageLoaderSDL_Image::supportedType[4] = { "png", "jpg",
                                                         "bmp", "tga" };

  struct doTolower
  {
    char operator()(char c) const {
      return tolower( c );
    }
  };

  ImageLoader *
  ImageLoader::getImageLoader( const std::string &ext )
  {
    using namespace std;
    string lowerCase = ext;
    transform( lowerCase.begin(), lowerCase.end(), lowerCase.begin(),
               doTolower() );
    if ( ImageLoaderSDL_Image::supportType( lowerCase ) )
      return new ImageLoaderSDL_Image;
    else if ( ImageLoaderDat::supportType( lowerCase ) )
      return new ImageLoaderDat;
    else {
      std::string mess("ImageLoader::getImageLoader: No support for file type: ");
      mess.append(ext);
      throw new Exception(mess);
    }
  }

  ImageLoader::ImageData *
  ImageLoaderSDL_Image::load( const std::string &filename )
  {
    ImageLoader::ImageData *imgData = new ImageData;
    SDL_Surface *image = IMG_Load(filename.c_str());

    if(image) {
      int bpp = image->format->BytesPerPixel;
      switch(bpp) {
      case 4:
        imgData->pixelFormat = PF_RGBA;
        break;
      case 3:
        imgData->pixelFormat = PF_RGB;
        break;
      case 1:
        imgData->pixelFormat = PF_ALPHA8;
        break;
      default:
        assert( 0 );
      }
      imgData->width = image->w;
      imgData->height = image->h;
      imgData->depth = 1;

      SDL_LockSurface(image);
      unsigned char *data = new unsigned char[image->w*image->h*bpp];
      unsigned char *pixels = (unsigned char *)image->pixels;
      imgData->data = ( void * )data;
      for ( int j = 0; j < image->h; j++ )
        memcpy( data+j*image->w*bpp, pixels+j*image->pitch, image->w*bpp );
      SDL_UnlockSurface(image);
      SDL_FreeSurface(image);
    } else {
      std::string mess("ImageLoaderSDL_Image::load: Can't load image ");
      mess.append(filename);
      throw new Exception(mess);
    }
    return imgData;
  }

  bool
  ImageLoaderSDL_Image::supportType( const std::string &lowerCase )
  {
    for ( size_t i = 0; i < sizeof( supportedType )/sizeof( char* ); i++ )
      if ( lowerCase.compare( supportedType[i] ) == 0 )
        return true;
    return false;
  }
}
