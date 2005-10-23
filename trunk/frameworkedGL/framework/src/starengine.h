#ifndef STARENGINE_H
#define STARENGINE_H

#pragma comment (lib, "frameworkGL.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "libfreetype.lib")
#pragma comment (lib, "SDL.lib")
#pragma comment (lib, "SDL_image.lib")
#pragma comment (lib, "SDLmain.lib")
#pragma comment (lib, "cg.lib")
#pragma comment (lib, "cgGL.lib")
#pragma comment (lib, "glew32s.lib")
#define GLEW_STATIC

#include "math/matrix.h"
#include "math/vector2.h"
#include "math/vector3.h"
#include "math/utils.h"
#include "math/quaternion.h"
#include "opengl/pixelformat.h"
#include "opengl/texture.h"
#include "opengl/geometricbatch.h"
#include "opengl/vertexbuffer.h"
#include "opengl/shadermanager.h"
#include "opengl/shaderCG.h"
#include "opengl/vertex.h"
#include "opengl/trackball.h"
#include "opengl/startypes.h"
#include "opengl/camera.h"
#include "opengl/material.h"
#include "opengl/light.h"
#include "opengl/mesh.h"
#include "opengl/font.h"
#include "opengl/texture3d.h"
#include "opengl/renderer.h"
#include "opengl/imageloader_dat.h"
#include "opengl/texture2d.h"
#include "opengl/texturemanager.h"
#include "opengl/image.h"
#include "opengl/imageloader.h"
#include "opengl/textureunit.h"
#include "opengl/shader.h"
#include "opengl/singleton.h"
#include "opengl/ressourcemanager.h"
#include "opengl/object3d.h"
#include "opengl/fbo.h"
#include "framework/application.h"
#include "framework/timer.h"
#include "framework/exception.h"

#endif
