#include <SDL_main.h>

#include "testapp.h"

int
main(int argc, char **argv)
{
  TestApp *app;
  int ret = -1;
  
  try {
    app = new TestApp();
    ret = app->run();
  } catch(Exception *e) {
    e->show();
  }

  return ret;
}
