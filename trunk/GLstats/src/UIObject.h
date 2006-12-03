#ifndef UIOBJECT_H
#define UIOBJECT_H

class UIObject {
public:
  virtual ~UIObject() {}
  virtual void draw() = 0;
};

#endif
