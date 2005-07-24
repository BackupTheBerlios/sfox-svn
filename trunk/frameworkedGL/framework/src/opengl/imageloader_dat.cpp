#ifdef _WIN32
#include <windows.h>
#endif
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <ctype.h>
#include <string>
#include <algorithm>

#include "opengl/imageloader_dat.h"
#include "framework/exception.h"

namespace StarEngine {
  const char *ImageLoaderDat::supportedType[1] = { "dat" };

  ImageLoader::ImageData *
  ImageLoaderDat::load( const std::string &filename )
  {
    ImageLoader::ImageData *imgData = new ImageData;
    char datafile[257];
    char tmp[257];
    FILE *file = fopen(filename.c_str(), "rt");
    fscanf(file, "ObjectFileName: %s\n", datafile);
    fscanf(file, "TaggedFileName: %s\n", tmp);
    fscanf(file, "Resolution: %d %d %d", &imgData->width, &imgData->height,
           &imgData->depth);
    printf("filename=%s %d %d %d\n", datafile, imgData->width, imgData->height,
           imgData->depth);
    fclose(file);

    return imgData;
  }

  bool
  ImageLoaderDat::supportType( const std::string &lowerCase )
  {
    for ( int i = 0; i < sizeof( supportedType )/sizeof( char* ); i++ )
      if ( lowerCase.compare( supportedType[i] ) == 0 )
        return true;
    return false;
  }
}
