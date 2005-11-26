#include <map>

#include "texturemanager.h"
#include "image.h"
#include "texture2d.h"
#include "texture3d.h"

namespace StarEngine {
  Texture *
  TextureManager::load(const std::string &filename, Texture *destTex,
                       int numMipmaps)
  {
    return load(filename, filename, destTex, numMipmaps);
  }

  Texture *
  TextureManager::create( const std::string &name, PixelFormat pf, int width,
                          int height, int depth)
  {
    Texture *tex;
    int dim = ( width>1 )+( height>1 )+( depth>1 );
    switch( dim ) {
    case 2:
      tex = new Texture2D(width, height, pf);
      break;
    case 3:
      tex = new Texture3D(width, height, depth, pf);
      break;
    default:
      assert( 0 );
    }
    tex->setName( name );
    ressources[name] = tex;
    return tex;
  }

  Texture *
  TextureManager::load(const std::string &name, const std::string &filename,
                       Texture *destTex, int numMipmaps)
  {
    Texture *tex = destTex;
    Image *img = new Image();
    img->load( filename );
    if ( img->getDimension() == 2 ) {
//      img->flipY();
      ImageLoader::ImageData *imgData = img->getImageData();
      if(!tex)
        tex = new Texture2D( imgData->pixelFormat );
      tex->setData( imgData->data, 0, imgData->pixelFormat, imgData->width,
                    imgData->height );
    } else if ( img->getDimension() == 3 ) {
      ImageLoader::ImageData *imgData = img->getImageData();
      if(!tex)
        tex = new Texture3D( imgData->pixelFormat );
      tex->setData( imgData->data, 0, imgData->pixelFormat, imgData->width,
                    imgData->height, imgData->depth );
    }
    if(!destTex) {
      tex->setMinFilter( TF_LINEAR );
      tex->setMagFilter( TF_LINEAR );
    }
    tex->setName( name );
    delete img;
    ressources[name] = tex;
    return tex;
  }

  Texture *
  TextureManager::getByName( const std::string &name )
  {
    return ( Texture * )RessourceManager::getByName( name );
  }

}
