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
    int w, h, d;
    ImageLoader::ImageData *imgData = new ImageData;
    char datafile[257];
    char tmp[257];
    FILE *file = fopen(filename.c_str(), "rt");
    fscanf(file, "ObjectFileName: %s\n", datafile);
    fscanf(file, "TaggedFileName: %s\n", tmp);
    fscanf(file, "Resolution: %d %d %d\n", &w, &h, &d);
    fclose(file);

    unsigned char *data = new unsigned char[w*h*d];
    int i = filename.find_last_of('/');
    if ( i != std::string::npos ) {
      strcpy(tmp, filename.substr(0, i+1).c_str() );
      strcat( tmp, datafile );
    } else
      strcpy(tmp, filename.c_str() );

    file = fopen(tmp, "rb");
    fread(data, 1, w*h*d, file);
    fclose(file);

    imgData->width = w;
    imgData->height = h;
    imgData->depth = d;
    imgData->data = data;
    imgData->pixelFormat = PF_LUMINANCE;

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
