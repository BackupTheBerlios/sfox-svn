#include <map>

#include "shadermanager.h"
#include "shaderCG.h"

namespace StarEngine {
  ShaderCG *
  ShaderManager::loadFragment(const std::string &name, const std::string &filename)
  {
    ShaderCG *shader = new ShaderCG();
    shader->loadSourceFromFile(filename.c_str(), ShaderCG::FRAGMENT);
    ressources[name] = shader;
    return shader;
  }
    
  ShaderCG *
  ShaderManager::loadVertex(const std::string &name, const std::string &filename)
  {
    ShaderCG *shader = new ShaderCG();
    shader->loadSourceFromFile(filename.c_str(), ShaderCG::VERTEX);
    ressources[name] = shader;
    return shader;
  }

  ShaderCG *
  ShaderManager::getByName( const std::string &name )
  {
    return ( ShaderCG * )RessourceManager::getByName( name );
  }

}
