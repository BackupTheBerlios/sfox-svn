#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "ressourcemanager.h"
#include "singleton.h"

namespace StarEngine {
  class ShaderCG;
}

namespace StarEngine {
  class ShaderManager:public RessourceManager, public Singleton<ShaderManager>{
  public:
    ShaderCG *getByName( const std::string &name );    
    ShaderCG *loadFragment(const std::string &name, const std::string &filename);
    ShaderCG *loadVertex(const std::string &name, const std::string &filename);
  };
#define g_ShaderManager ShaderManager::getSingleton()
}

#endif
