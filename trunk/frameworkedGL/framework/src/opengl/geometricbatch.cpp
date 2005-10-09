#ifdef WIN32
#include <windows.h>
#endif

#include <string>
#include <vector>
#include <map>
#include <GL/gl.h>

#include "geometricbatch.h"
#include "framework/exception.h"

namespace StarEngine {

/****************************************************************************/
/* Define the available types for vertex attributes                         */
/****************************************************************************/
  struct TypeDescription {
    char *name;
    int size;
    GLenum type;
  };

  static TypeDescription typeDesc[] = {
    {"float", sizeof(float), GL_FLOAT},
    {"double", sizeof(double), GL_DOUBLE},
    {"int", sizeof(int), GL_INT},
    {"uint", sizeof(unsigned int), GL_UNSIGNED_INT},
    {"short", sizeof(short), GL_SHORT},
    {"ushort", sizeof(unsigned short), GL_UNSIGNED_SHORT},
    {"byte", sizeof(short), GL_BYTE},
    {"ubyte", sizeof(unsigned short), GL_UNSIGNED_BYTE}
  };

/****************************************************************************/
/* Define the default binding for vertex attributes (NV's cg)               */
/****************************************************************************/
  struct AttribPos {
    char *name;
    int pos;
  };

  static AttribPos attribDefaultPos[] = {
    {"vertex", 0},
    {"blendweight", 1},
    {"normal", 2},
    {"color0", 3},
    {"color1", 4},
    {"tessfactor", 5},
    {"psize", 6},
    {"blendindices", 7},
    {"texcoord0", 8},
    {"texcoord1", 9},
    {"texcoord2", 10},
    {"texcoord3", 11},
    {"texcoord4", 12},
    {"texcoord5", 13},
    {"texcoord6", 14},
    {"tangent", 14},
    {"texcoord7", 15},
    {"binormal", 15}
  };

/****************************************************************************/
  GeometricBatch::GeometricBatch()
  {
  }

/****************************************************************************/
  void
  GeometricBatch::setVertexFormat(const std::string &format)
  {
    using namespace std;
    vector<FormatPair> vf = extractFormat( format );
    int size;
    int ind;
    getTypeDescription(vf[0].second, size, ind);
    cerr << typeDesc[ind].size << std::endl;

  }

/****************************************************************************/
/* Extract each word from the string "x:a y:b ..."                          */
/* Return a vector of pair (x,a) (y,b) ...                                  */
/****************************************************************************/
  std::vector<GeometricBatch::FormatPair>
  GeometricBatch::extractFormat(const std::string &format)
  {
    using namespace std;
    string::const_iterator i;
    vector<FormatPair> conf;
    string tmp;

    for(i = format.begin(); i != format.end(); i++) {
      char c = *i;
      bool notEmpty = !tmp.empty();
      if(c != ' ')
        tmp.append(1, c);
      else if(notEmpty || (notEmpty && i == format.end())) {
        unsigned int pos;
        if( (pos = tmp.find_first_of(':')) == string::npos)
          throw new Exception("GeometricBatch::extractFormat: invalid format");
        conf.push_back(FormatPair(tmp.substr(0, pos), tmp.substr(pos+1)) );
        tmp.clear();
      }
    }

    return conf;
  }

  void
  GeometricBatch::getTypeDescription(const std::string &type,
                                     int &numComponents, int &typeIndice)
  {
    int len = type.size();
    int i = 0;
    while(i < len && !isdigit(type[i]))
      i++;

    for(size_t j = 0; j < sizeof(typeDesc)/sizeof(typeDesc[0]); j++)
      if(type.compare(0, i, typeDesc[j].name) == 0) {
        numComponents = atoi(type.c_str()+i);
        typeIndice = j;
        return;
      }
    std::string mess("GeometricBatch::getTypeDescription: unknow type \"");
    mess.append(type);
    mess.append("\"");
    throw new Exception(mess);
  }
}
