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
Mipmap::getTexture(Texture2D *texture, int xoffs, int yoffs,
                   int width, int height)
{
  size_t numLevels = levels.size();
  for(size_t l = 0; l < numLevels; l++) {
    ImageLoader::ImageData *imgData = levels[l]->getImageData();
    imgData->pixelFormat = PF_LUMINANCE;
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, xoffs);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, yoffs);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, imgData->width);
    texture->setData(imgData->data, l, imgData->pixelFormat, width, height);
    width /= 2;
    height /= 2;
    xoffs /= 2;
    yoffs /= 2;
  }
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

  //j==0&&i==0
  dst[ofs++] = (src[0]+src[1]+src[oldw]+src[1+oldw])/4.0f;
  for(int i = 2; i < oldw-1; i+=2) {
    int s = src[i-1]+src[i]+src[i+1]+
            src[i-1+oldw]+src[i+oldw]+src[i+1+oldw];
    dst[ofs++] = s/6.0f;
  }

  for(int j = 2; j < oldh-1; j+=2) {
    //i==0
    dst[ofs++] = (src[(j-1)*oldw]+src[1+(j-1)*oldw]+
                  src[j*oldw]+src[1+j*oldw]+
                  src[(j+1)*oldw]+src[1+(j+1)*oldw])/6.0f;
    for(int i = 2; i < oldw-1; i+=2) {
      int s = src[i-1+(j-1)*oldw]+src[i+(j-1)*oldw]+src[i+1+(j-1)*oldw]+
              src[i-1+j*oldw]+src[i+j*oldw]+src[i+1+j*oldw]+
              src[i-1+(j+1)*oldw]+src[i+(j+1)*oldw]+src[i+1+(j+1)*oldw];
      dst[ofs++] = s/9.0f;
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
