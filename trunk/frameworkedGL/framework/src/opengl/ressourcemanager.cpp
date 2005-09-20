#include <iostream>
#include "ressourcemanager.h"

namespace StarEngine {
  Ressource *
  RessourceManager::getByName( const std::string &name )
  {
    using namespace std;
    RessourceMap::iterator i = ressources.find( name );
    if( i != ressources.end() )
      return ( *i ).second;
    return 0;
  }

  void
  RessourceManager::releaseRessource( const std::string &name )
  {
    using namespace std;
    RessourceMap::iterator i = ressources.find( name );
    if( i != ressources.end() )
      delete ( *i ).second;    
  }
};
