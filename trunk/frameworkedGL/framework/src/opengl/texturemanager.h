#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "ressourcemanager.h"
#include "singleton.h"

namespace StarEngine {
  class Texture;
}

namespace StarEngine {
  class TextureManager:public RessourceManager, public Singleton<TextureManager>{
  public:
    Texture *getByName( const std::string &name );
    Texture *load(const std::string &filename, int numMipmaps = 0);
    Texture *load(const std::string &name, const std::string &filename,
                  int numMipmaps = 0);
  };
#define g_TextureManager TextureManager::getSingleton()
}

#endif
