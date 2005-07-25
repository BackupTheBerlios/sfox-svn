#include <map>

#include "texturemanager.h"
#include "image.h"
#include "texture2d.h"
#include "texture3d.h"

namespace StarEngine {
  Texture *
  TextureManager::load(const std::string &filename, int numMipmaps)
  {
    return load(filename, filename, numMipmaps);
  }

  Texture *
  TextureManager::load(const std::string &name, const std::string &filename,
                       int numMipmaps)
  {
    Texture *tex;
    Image *img = new Image();
    img->load( filename );
    if ( img->getDimension() == 2 ) {
      img->flipY();
      ImageLoader::ImageData *imgData = img->getImageData();
      tex = new Texture2D( imgData->pixelFormat );
      tex->setData( imgData->data, imgData->pixelFormat, imgData->width,
                    imgData->height );
      tex->setMinFilter( TF_LINEAR );
      tex->setMagFilter( TF_LINEAR );
    } else if ( img->getDimension() == 3 ) {
      ImageLoader::ImageData *imgData = img->getImageData();
      tex = new Texture3D( imgData->pixelFormat );
      tex->setData( imgData->data, imgData->pixelFormat, imgData->width,
                    imgData->height, imgData->depth );
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
