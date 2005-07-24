#ifndef SINGLETON_H
#define SINGLETON_H

#include <cassert>
#include <iostream>

// Scott Bilas

template <typename T>
class Singleton {
  static T* instance;
public:
  Singleton() {
    assert(!instance);
    int offset = (int)(T*)1 - (int)(Singleton<T> *)(T*)1;
    instance = (T*)((int)this + offset);
  }

  ~Singleton() {
    instance = 0;
  }

  static T& getSingleton() {
    return *instance;
  }

  static T* getSingletonPtr() {
    return instance;
  }
};

template <typename T> T* Singleton <T>::instance = 0;
#endif
