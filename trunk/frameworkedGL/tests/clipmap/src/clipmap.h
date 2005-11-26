#ifndef CLIPMAP_H
#define CLIPMAP_H

#include "starengine.h"

class Mipmap;

class ClipMap {
public:
  ClipMap(int clipmapSize, int numLevels);
  ~ClipMap();

  void render();
  void drawDebugMipmap(int levelToDisplay);

  void setWireframe(bool enable) { wireframe = enable; }
  bool isWireframe() { return wireframe; }

private:
  StarEngine::GeometricBatch *blockVertices;
  StarEngine::IndicesBatch *blockIndices;

  StarEngine::GeometricBatch *ringFixupVertices;
  StarEngine::IndicesBatch *ringFixupIndices;

  StarEngine::GeometricBatch *tlFixupVertices;
  StarEngine::IndicesBatch *tlFixupIndices;

  StarEngine::GeometricBatch *finestLevelVertices;
  StarEngine::IndicesBatch *finestLevelIndices;

  StarEngine::EffectCG *clipmapFX;
  CGtechnique technique;
  CGtechnique techniqueNoFrag;

  //Generate vertex and indices buffers for biggest blocks
  void genBlocks();
  //Generate vertex and indices buffers for ring fixup
  void genRingFixUp();
  //Generate vertex and indices buffers for central mesh
  void genFinestLevel();
  //Generate vertex and indices buffers for L-shaped fixups
  void genL();

  void drawBlocks(int level);
  void drawRingFixup(int level);
  void drawFinestLevel();
  void drawLFixup(int level);

  int clipmapSize;
  int numLevels;

  Mipmap *mipmap;
  std::vector<StarEngine::Texture2D *> geomTex;
  std::vector<StarEngine::Texture2D *> heightTex;

  bool wireframe;
  void drawWireframe(StarEngine::GeometricBatch *vertices,
                     StarEngine::IndicesBatch *indices, int level);
};

#endif
