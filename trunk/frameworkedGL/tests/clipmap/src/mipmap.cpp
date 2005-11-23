#include "mipmap.h"

using namespace StarEngine;

void
Mipmap::buildMipmap(const char *filename, int numLevels)
{
  levels.push_back(new Image);
  levels[0]->load(filename);
  Timer timer;
  timer.start();
  for(int i = 1; i < numLevels; i++) {
    levels.push_back(halfScale(levels[i-1]));
  }
  printf("Mipmap gentime=%dms\n", timer.getElapsed());
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

void
Mipmap::buildMipmapGPU(const char *filename, int numLevels)
{
  FramebufferObject *fbo = new FramebufferObject;
  Texture2D *destTex = new Texture2D(PF_RGB);
  Texture2D *sourceTex = new Texture2D(PF_RGB);

  levels.push_back(new Image);
  levels[0]->load(filename);
  Timer timer;
  timer.start();

  ImageLoader::ImageData *imgData = levels[0]->getImageData();
  int width = imgData->width;
  int height = imgData->height;
  sourceTex->setData(imgData->data, 0, PF_LUMINANCE, width, height);
  sourceTex->setMinFilter(TF_LINEAR);
  sourceTex->setMagFilter(TF_LINEAR);

  glPushMatrix();
  int vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  fbo->bind();
  for(int i = 1; i < numLevels; i++) {
    int newWidth = width/2;
    int newHeight = height/2;

    glViewport(0, 0, newWidth, newHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, newWidth-1, newHeight-1, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    destTex->setData(NULL, 0, PF_RGB, newWidth, newHeight);
    TextureUnits::activeUnit( 0 );
    TextureUnits::setEnvMode( TEM_REPLACE );

    fbo->attachTexture(destTex, FramebufferObject::COLOR_ATTACHMENT0, 0);
    fbo->checkStatus();
    Renderer::printGLError();

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    sourceTex->bind();
    glEnable(GL_TEXTURE_2D);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(1,1,0);
    glBegin(GL_QUADS);
    glTexCoord2f(0., 1.);
    glVertex2f(0, 0);
    glTexCoord2f(0., 0.);
    glVertex2f(0, newHeight-1);
    glTexCoord2f(1., 0.);
    glVertex2f(newWidth-1, newHeight-1);
    glTexCoord2f(1., 1.);
    glVertex2f(newWidth-1, 0);
    glEnd();

    ImageLoader::ImageData *newImg = new ImageLoader::ImageData;
    newImg->pixelFormat = PF_LUMINANCE;
    newImg->width = newWidth;
    newImg->height = newHeight;
    newImg->data = new unsigned char[newWidth*newHeight];
    glReadPixels(0, 0, newWidth, newHeight, GL_RED, GL_UNSIGNED_BYTE,
                 newImg->data);

    Image *res = new Image;
    res->setImageData(newImg);
    levels.push_back(res);

    Texture2D *tmp;
    tmp = destTex;
    destTex = sourceTex;
    sourceTex = tmp;

    width = newWidth;
    height = newHeight;
  }
  fbo->unbind();

  glViewport(vp[0], vp[1], vp[2], vp[3]);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  printf("Mipmap gentime=%dms\n", timer.getElapsed());

  delete fbo;
  delete destTex;
  delete sourceTex;
}

/****************************************************************************/

Image *
Mipmap::halfScaleGPU(Image *img)
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
