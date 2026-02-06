#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>
#include <string>

#include <emscripten/html5.h>
#include <glm/glm.hpp>

namespace input {
  const float dragDist = 10.0f;

  enum actions {
    forward,
    backward,
    left,
    right,
    up,
    down,
    click,
    shift,
    freeLook,
    freeCam,
    zoomIn,
    zoomOut,
    revertCamera,
    perspective,
    mainCamera,
    pointCamera,
    topCamera,
    camera1,
    camera2,
    camera3,
    camera4,
  };

extern std::unordered_map<std::string, actions> keyToAction;

} // namespace input

class Mouse {
public:
  double x = 0.0;
  double y = 0.0;
  double dx = 0.0;
  double dy = 0.0;

  glm::vec2 mouseDownPos = glm::vec2(0.0f);
  bool drag = false;

  void update(std::unordered_map<input::actions, bool>& actionCurr);

  bool first = true;
  bool pointerLock = false;
private:
  // void recenter();
};


class Input {
public:
  static Mouse mouse;
  static std::unordered_map<input::actions, bool> actionPrev;
  static std::unordered_map<input::actions, bool> actionCurr;

  bool pressed(input::actions a) const;
  bool justPressed(input::actions a) const;
  bool justReleased(input::actions a) const;

  static EM_BOOL keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData);
  static EM_BOOL mouseMoveCallback(int eventType, const EmscriptenMouseEvent* e, void* userData);
  static EM_BOOL mouseButtonCallback(int eventType, const EmscriptenMouseEvent* e, void* userData);
  static EM_BOOL mouseWheelCallback(int eventType, const EmscriptenWheelEvent* e, void* userData);

  void update();
  void setup();

  void clear();
private:
};



#endif
