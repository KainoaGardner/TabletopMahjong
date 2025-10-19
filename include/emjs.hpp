#ifndef EMJS_H
#define EMJS_H

#include <emscripten.h>

extern "C" {
void hideMouse();
void showMouse();
double getCanvasMouseX(int clientX);
double getCanvasMouseY(int clientY);
void preventPageScroll();
}

#endif
