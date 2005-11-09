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

  StarEngine::GeometricBatch *finestLevelVertices;
  StarEngine::IndicesBatch *finestLevelIndices;

  StarEngine::ShaderCG *clipmapVert;
  StarEngine::ShaderCG *clipmapFrag;

  //Generate vertex and indices buffers for biggest blocks
  void genBlocks();
  //Generate vertex and indices buffers for ring fixup
  void genRingFixUp();
  //Generate vertex and indices buffers for central mesh
  void genFinestLevel();

  void drawBlocks();
  void drawRingFixup();
  void drawFinestLevel();

  int clipmapSize;
};

#endif
