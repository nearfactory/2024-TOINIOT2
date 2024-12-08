#pragma once

#include <Arduino.h>

// 機体の座標について
// コートが122x183(cm)より、
// x:-1~1, y:-1.5~1.5 とする

class Camera{
private:
public:
  Vec2 position;

};

extern Camera camera;