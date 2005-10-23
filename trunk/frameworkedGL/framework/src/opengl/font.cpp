#ifdef WIN32
#include <windows.h>
#define vsnprintf(b, n, f, l) vsprintf(b, f, l)
#endif

#include <GL/glew.h>
#include <GL/glu.h>
#include <string>
#include <cassert>
#include <clocale>
#include <cctype>
#include <cstdarg>

#include "font.h"
#include "texture2d.h"
#include "textureunit.h"
#include "framework/exception.h"
#include "renderer.h"

namespace StarEngine {
  FT_Library FontGL::ftLibrary = 0;

  FontGL::FontGL(const char *filename, int pixelSize, int textureSize)
    : textureSize(textureSize)
  {
    if(!ftLibrary)
      initLibrary();

    setColor(1, 0, 0);
    //Load the font file
    FT_Error error = FT_New_Face(ftLibrary, filename, 0, &face);
    if(error) {
      std::string mess("FontGL: Can't load font face for ");
      throw new Exception(mess.append(filename));
    }

    //Set char size
    error = FT_Set_Pixel_Sizes(face, pixelSize, pixelSize);
    if(error)
      throw new Exception("FontGL: Can't set char size.");

    generateTexture();
    //setlocale(LC_ALL, "");
  }

  FontGL::~FontGL()
  {
    FT_Done_Face(face);
  }

  void
  FontGL::generateTexture()
  {
    using namespace std;
    FT_Glyph glyph;
    int u = 0, v = 0, maxHeight = 0;

    Texture *texCurrent = new Texture2D(textureSize, textureSize, PF_ALPHA8);
    texCurrent->setMinFilter( TF_NEAREST );
    texCurrent->setMagFilter( TF_NEAREST );

    textures.push_back(texCurrent);
    texCurrent->bind();

    for(int c = 0; c < 255; c++) {
      if(!isprint(c))
        continue;
      FT_Error error = FT_Load_Glyph(face, FT_Get_Char_Index(face, c),
                                     FT_LOAD_DEFAULT);
      if(error)
        throw new Exception("FontGL: Can't load gyph.");

      error = FT_Get_Glyph(face->glyph, &glyph);
      if(error)
        throw new Exception("FontGL: Can't get gyph.");

      error = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 0);
      if(error)
        throw new Exception("FontGL: Can't convert gyph to bitmap.");

      FT_Bitmap bmp = ((FT_BitmapGlyph)glyph)->bitmap;

      //Do we need to go to the next line
      if(u+bmp.width >= textureSize) {
        u = 0;
        v += maxHeight;
        maxHeight = 0;
      }
      //Do we need to go to the next texture
      if(v+bmp.rows >= textureSize) {
        u = v = maxHeight = 0;
        texCurrent = new Texture2D(textureSize, textureSize, PF_ALPHA8);
        texCurrent->setMinFilter( TF_NEAREST );
        texCurrent->setMagFilter( TF_NEAREST );

        textures.push_back(texCurrent);
        texCurrent->bind();
      }
      maxHeight = max(maxHeight, bmp.rows);

      addLetter(c, glyph, bmp.width, bmp.rows, u, v);
      texCurrent->subImage(bmp.buffer, PF_ALPHA8, bmp.width,
                           bmp.rows, 0, u, v);
      u += bmp.width;

      FT_Done_Glyph(glyph);
    }
  }

  void
  FontGL::addLetter(int c, FT_Glyph glyph, unsigned int width,
                    unsigned int height, int u, int v)
  {
    letter *tmp = &letters[c];
    FT_Glyph_Get_CBox(glyph, ft_glyph_bbox_pixels, &tmp->bbox);
    FT_BitmapGlyph bmp = (FT_BitmapGlyph)glyph;
    tmp->left = bmp->left;
    tmp->top = bmp->top;
    tmp->minU = (float)u/(float)textureSize;
    tmp->minV = (float)v/(float)textureSize;
    tmp->maxU = (float)(u+width)/(float)textureSize;
    tmp->maxV = (float)(v+height)/(float)textureSize;
    tmp->width = width; tmp->height = height;
    tmp->advance = face->glyph->advance;
    tmp->texture = (int)textures.size()-1;
  }

/*Save openGL states, set color, set matrix*/
  void
  FontGL::begin()
  {
    glPushAttrib(GL_DEPTH_BUFFER_BIT|GL_ENABLE_BIT|GL_POLYGON_BIT);
    glDisable(GL_TEXTURE_3D);
    glDisable(GL_TEXTURE_RECTANGLE_ARB);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glColor3fv(color);
  }

/*Restor openGL states and proj matrix*/
  void
  FontGL::end()
  {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
  }

  void
  FontGL::drawChar(float x, float y, int c)
  {
    letter *tmp = &letters[c];
    //Do we need to change active texture?
    if(currentTexture != tmp->texture) {
      currentTexture = tmp->texture;
      textures[currentTexture]->bind();
      TextureUnits::setEnvMode( TEM_BLEND );
    }
    x += tmp->left;
    y -= tmp->height-tmp->top;

    glLoadIdentity();
    glBegin(GL_QUADS);
    glTexCoord2f(tmp->minU, tmp->maxV);
    glVertex2f(x, y);

    glTexCoord2f(tmp->minU, tmp->minV);
    glVertex2f(x, y+tmp->height);

    glTexCoord2f(tmp->maxU, tmp->minV);
    glVertex2f(x+tmp->width, y+tmp->height);

    glTexCoord2f(tmp->maxU, tmp->maxV);
    glVertex2f(x+tmp->width, y);
    glEnd();
  }

  void
  FontGL::printf(float x, float y, const char *fmt, ...)
  {
    char buffer[MAXSTRINGLEN];
    va_list liste;

    va_start(liste, fmt);
    vsnprintf(buffer, MAXSTRINGLEN, fmt, liste);
    va_end(liste);

    buffer[MAXSTRINGLEN-1] = '\0';
    drawLine(x, y, buffer);
  }

  void
  FontGL::drawLine(float x, float y, const char *str)
  {
    FT_UInt prevGlyph = 0;

    FT_Long useKerning = FT_HAS_KERNING(face);
    currentTexture = -1;
    begin();
    for(;*str != '\0';str++) {
      if(!isprint(*str))
        continue;
      FT_UInt curGlyph = FT_Get_Char_Index(face, *str);
      if(useKerning&&prevGlyph) {
        FT_Vector kern;
        FT_Get_Kerning(face, prevGlyph, curGlyph, FT_KERNING_UNFITTED, &kern);
        x += kern.x >> 6;
      }
      drawChar(x, y, *str);
      prevGlyph = curGlyph;
      x += letters[(unsigned int)*str].advance.x>>6;
    }
    end();
  }

  void
  FontGL::setColor(float r, float g, float b)
  {
    color[0] = r; color[1] = g; color[2] = b;
  }

/*****************************************************************************/
/* Static functions for init/exit of freetype lib                            */
/*****************************************************************************/
  void
  FontGL::initLibrary()
  {
    FT_Error error = FT_Init_FreeType(&ftLibrary);
    if (error)
      throw new Exception("Can't initializate Freetype");
  }

  void
  FontGL::exit()
  {
    if(ftLibrary)
      FT_Done_FreeType(ftLibrary);
  }


/*****************************************************************************/
/*DEBUG                                                                      */
/*****************************************************************************/
  void
  FontGL::testRender()
  {
    begin();
    textures[0]->bind();
    glEnable(GL_BLEND);
    TextureUnits::setEnvMode( TEM_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(0,0);

    glTexCoord2f(0, 1);
    glVertex2f(0, (float)textureSize);

    glTexCoord2f(1, 1);
    glVertex2f((float)textureSize, (float)textureSize);

    glTexCoord2f(1, 0);
    glVertex2f((float)textureSize, 0);
    glEnd();
    end();
  }
}
