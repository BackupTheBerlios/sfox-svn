#ifndef RESSOURCEMANAGER_H
#define RESSOURCEMANAGER_H

#include <string>
#include <map>

namespace StarEngine {
  class Ressource {
  protected:
    std::string ressourceName;
  public:
    void setName(const std::string &name) { ressourceName = name; }
  };

  class RessourceManager {
  public:
    Ressource *getByName( const std::string &name );
    void releaseRessource( const std::string &name );

  protected:
    typedef std::map<std::string, Ressource *> RessourceMap;
    RessourceMap ressources;
  };
}

#endif
