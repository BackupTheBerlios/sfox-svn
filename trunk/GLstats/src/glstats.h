#ifndef GLSTATS_H
#define GLSTATS_H

#include "font.h"
#include "timer.h"

extern FontGL *fontGL;
extern Timer *timer;

extern void GLStatsInit();
extern void GLStatsExit();

extern void GLStatsInitGL();
extern void GLStatsSwapBuffers();

#endif
