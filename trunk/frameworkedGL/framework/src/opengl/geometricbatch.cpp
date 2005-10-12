#ifdef WIN32
#include <windows.h>
#endif

#include <string>
#include <cassert>
#include <vector>
#include <map>
#include <GL/gl.h>
#include <GL/glext.h>

#include "geometricbatch.h"
#include "framework/exception.h"
#include "glext.h"

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
    const char *name;
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
    :verticesBufferId(0), indicesBufferId(0), numVertices(0)
  {
  }

/****************************************************************************/
  GeometricBatch::~GeometricBatch()
  {
    if(indicesBufferId)
      glDeleteBuffersARB(1, &verticesBufferId);

    if(verticesBufferId)
      glDeleteBuffersARB(1, &indicesBufferId);
  }

/****************************************************************************/
  void
  GeometricBatch::drawArrays(int first, int count)
  {
    assert(verticesBufferId != 0 && primitiveMode != PM_UNKNOWN &&
           numVertices != 0);
    count = count == -1?numVertices:count;
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, verticesBufferId);
    glDrawArrays(getGLPrimitiveMode(primitiveMode), first, count);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
  }

/****************************************************************************/
  void
  GeometricBatch::setVertices(int size, void *data, GLenum usage)
  {
    numVertices = size/computeStride(extractFormat(vertexFormat));
    printf("%d\n", numVertices);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, verticesBufferId);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, usage);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
  }

/****************************************************************************/
  void
  GeometricBatch::setIndices(int size, void *data, GLenum usage)
  {
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indicesBufferId);
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, size, data, usage);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
  }

/****************************************************************************/
  void
  GeometricBatch::setVertexFormat(const std::string &format)
  {
    using namespace std;
    vector<FormatPair> vf = extractFormat( format );
    vertexFormat = format;

    glGenBuffersARB(1, &verticesBufferId);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, verticesBufferId);

    int numComponent;
    int ind;
    int stride = computeStride(vf);
    char *offset = 0;
    for(size_t i = 0; i < vf.size(); i++) {
      getTypeDescription(vf[i].second, numComponent, ind);
      int pos = getAttribDefaultPos(vf[i].first);
      glVertexAttribPointerARB(pos, numComponent, typeDesc[ind].type,
                               GL_FALSE, stride, offset);
      offset += numComponent*typeDesc[ind].size;
    }
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
  }

/****************************************************************************/
  void
  GeometricBatch::setPrimitiveMode(PrimitiveMode pt)
  {
    primitiveMode = pt;
  }

/****************************************************************************/
  GLenum
  GeometricBatch::getGLPrimitiveMode(PrimitiveMode pt)
  {
    switch(pt) {
    case PM_POINTS:
      return GL_POINTS;
    case PM_LINE_STRIP:
      return GL_LINE_STRIP;
    case PM_LINE_LOOP:
      return GL_LINE_LOOP;
    case PM_LINES:
      return GL_LINES;
    case PM_TRIANGLE_STRIP:
      return GL_TRIANGLE_STRIP;
    case PM_TRIANGLE_FAN:
      return GL_TRIANGLE_FAN;
    case PM_TRIANGLES:
      return GL_TRIANGLES;
    case PM_QUAD_STRIP:
      return GL_QUAD_STRIP;
    case PM_QUADS:
      return GL_QUADS;
    case PM_POLYGON:
      return GL_POLYGON;
    default:
      assert(0);
    };
    assert(0);
    return 0;
  }

/****************************************************************************/
  int
  GeometricBatch::computeStride(std::vector<FormatPair> vf)
  {
    int numComponent;
    int ind;
    int stride = 0;
    for(size_t i = 0; i < vf.size(); i++) {
      getTypeDescription(vf[i].second, numComponent, ind);
      stride += numComponent*typeDesc[ind].size;
    }
    return stride;
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
      if((c == ' ' && notEmpty) || (notEmpty && i == format.end()-1)) {
        unsigned int pos;
        if( (pos = tmp.find_first_of(':')) == string::npos)
          throw new Exception("GeometricBatch::extractFormat: invalid format");
        conf.push_back(FormatPair(tmp.substr(0, pos), tmp.substr(pos+1)) );
        tmp.clear();
      }
    }

    return conf;
  }

/****************************************************************************/
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

/****************************************************************************/
  int
  GeometricBatch::getAttribDefaultPos(const std::string &name)
  {
    for(size_t i = 0; i < sizeof(attribDefaultPos)/sizeof(attribDefaultPos[0]); i++)
    {
      if(name.compare(attribDefaultPos[i].name) == 0)
        return attribDefaultPos[i].pos;
    }
    std::string mess("GeometricBatch::getAttribDefaultPos: unknow default pos for attrib \"");
    mess.append(name);
    mess.append("\"");
    throw new Exception(mess);
    return -1;
  }
}
