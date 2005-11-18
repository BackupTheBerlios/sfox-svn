#include "mipmap.h"

using namespace StarEngine;

void
Mipmap::buildMipmap(const char *filename, int numLevels)
{
  levels.push_back(new Image);
  levels[0]->load(filename);
  for(int i = 1; i < numLevels; i++) {
    levels.push_back(halfScale(levels[i-1]));
  }
}
/****************************************************************************/

void
Mipmap::getTextures(std::vector<Texture2D *> &textures,
                    int xoffs, int yoffs, int width, int height)
{
  size_t numLevels = levels.size();
  for(size_t l = 0; l < numLevels; l++) {
    Texture2D *tex = new Texture2D(PF_LUMINANCE32F);
    textures.push_back(tex);
    ImageLoader::ImageData *imgData = levels[l]->getImageData();
    imgData->pixelFormat = PF_LUMINANCE;
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, xoffs);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, yoffs);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, imgData->width);
    tex->setData(imgData->data, 0, imgData->pixelFormat, width, height);
    xoffs = xoffs/2-width/4;
    yoffs = yoffs/2-width/4;
    tex->setWrapS( TW_CLAMP_TO_EDGE );
    tex->setWrapT( TW_CLAMP_TO_EDGE );
  }
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
}
/****************************************************************************/

Image *
Mipmap::halfScale(Image *img)
{
  Image *res = new Image;
  ImageLoader::ImageData *oldImg = img->getImageData();
  ImageLoader::ImageData *newImg = new ImageLoader::ImageData;
  newImg->pixelFormat = oldImg->pixelFormat;
  int oldw = newImg->width = oldImg->width;
  int oldh = newImg->height = oldImg->height;
  int w = newImg->width = oldw/2;
  int h = newImg->height = oldh/2;
  int size = PixelFormatUtils::getBytesPerPixel(newImg->pixelFormat)*w*h;
  newImg->data = new unsigned char[size];

  unsigned char *dst = (unsigned char *)newImg->data;
  unsigned char *src = (unsigned char *)oldImg->data;
  int ofs = 0;

  for(int j = 0; j < oldh-1; j+=2) {
    for(int i = 0; i < oldw-1; i+=2) {
      int s = src[i+j*oldw]+src[i+1+j*oldw]+src[i+(j+1)*oldw]+
              src[i+1+(j+1)*oldw];
      dst[ofs++] = s/4.0f;
    }
  }
  res->setImageData(newImg);
  return res;
}

/****************************************************************************/

Image *
Mipmap::getLevel(int n)
{
  return levels[n];
}
