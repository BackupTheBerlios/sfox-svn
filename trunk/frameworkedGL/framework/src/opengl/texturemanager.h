#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "ressourcemanager.h"
#include "singleton.h"
#include "pixelformat.h"

namespace StarEngine {
  class Texture;
}

namespace StarEngine {
  class TextureManager:public RessourceManager, public Singleton<TextureManager>{
  public:
    Texture *getByName( const std::string &name );
    Texture *create( const std::string &name, PixelFormat pf, int width,
                    int height = 1, int depth = 1);
    Texture *load(const std::string &filename, Texture *destTex = 0,
                  int numMipmaps = 0);
    Texture *load(const std::string &name, const std::string &filename,
                  Texture *destTex = 0, int numMipmaps = 0);
  };
#define g_TextureManager TextureManager::getSingleton()
}

#endif
