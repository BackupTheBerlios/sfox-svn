#ifndef CLIPMAP_H
#define CLIPMAP_H

#include "starengine.h"

class ClipMap {
public:
  ClipMap(int n);
  ~ClipMap();

  void render();

private:
  StarEngine::GeometricBatch *blockVertices;
  StarEngine::IndicesBatch *blockIndices;

  StarEngine::GeometricBatch *ringFixupVertices;
  StarEngine::IndicesBatch *ringFixupIndices;

  StarEngine::ShaderCG *clipmapVert;
  StarEngine::ShaderCG *clipmapFrag;

  //Generate vertex and indices buffers for biggest blocks
  void genBlocks();
  //Generate vertex and indices buffers for ring fixup
  void genRingFixUp();

  void drawBlocks();
  void drawRingFixup();

  int clipmapSize;
};

#endif
