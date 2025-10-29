#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>
#include <string>

#include <emscripten/html5.h>

namespace input {
  enum actions {
    forward,
    backward,
    left,
    right,
    up,
    down,
    click,
    freeLook,
    zoomIn,
    zoomOut,
    perspective,
  };

extern std::unordered_map<std::string, actions> keyToAction;
extern std::unordered_map<actions, bool> actionPressed;

EM_BOOL keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData);
EM_BOOL mouseMoveCallback(int eventType, const EmscriptenMouseEvent* e, void* userData);
EM_BOOL mouseButtonCallback(int eventType, const EmscriptenMouseEvent* e, void* userData);
EM_BOOL mouseWheelCallback(int eventType, const EmscriptenWheelEvent* e, void* userData);

struct Mouse {
  double x = 0.0;
  double y = 0.0;
  double dx = 0.0;
  double dy = 0.0;
  bool pointerLock = false;
  bool first = true;

  bool justClicked = false;
  long long clickTime = 0.0;
};

extern Mouse mouse;

void updateMouse();
void recenterMouse();

void setup();

} // namespace input

#endif
