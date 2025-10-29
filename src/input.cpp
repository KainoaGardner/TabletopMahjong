#include "../include/input.hpp"
#include "../include/emjs.hpp"

#include <chrono>

namespace input {

std::unordered_map<std::string, actions> keyToAction = {
  {"KeyW", forward},
  {"KeyS", backward},
  {"KeyA", left},
  {"KeyD", right},
  {"KeyQ", up},
  {"KeyE", down},
  {"Mouse0", click},
  {"Mouse2", freeLook},
  {"MouseWheelUp", zoomIn},
  {"MouseWheelDown", zoomOut},
  {"KeyP", perspective},
};

std::unordered_map<actions, bool> actionPressed = {
  {forward, false},
  {backward, false},
  {left, false},
  {right, false},     
  {up, false},
  {down, false},     

};

EM_BOOL keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData){
  auto it = keyToAction.find(e->code);
  if (eventType == EMSCRIPTEN_EVENT_KEYDOWN){
    if (it != keyToAction.end()){
      actionPressed[it->second] = true;
    }

    if (it->second == click){
      mouse.justClicked = true;

      auto now = std::chrono::system_clock::now();
      auto duration = now.time_since_epoch();
      long long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
      mouse.clickTime = currentTime;
    }
    if (it->second == freeLook){
      mouse.dx = 0.0;
      mouse.dy = 0.0;
      mouse.first = true;
    }


  }else if (eventType == EMSCRIPTEN_EVENT_KEYUP){
    if (it != keyToAction.end()){
      actionPressed[it->second] = false;
    }
  }

  return EM_TRUE;
}


Mouse mouse;

EM_BOOL mouseMoveCallback(int eventType, const EmscriptenMouseEvent* e, void* userData){
    double canvasX = e->canvasX;
    double canvasY = e->canvasY;
  
    double clientX = e->clientX;
    double clientY = e->clientY;
    double currX = getCanvasMouseX(clientX);
    double currY = getCanvasMouseY(clientY);

  
    if (mouse.first){
      mouse.x = currX;
      mouse.y = currY;
      mouse.dx = 0.0;
      mouse.dy = 0.0;

      mouse.first = false;
      return EM_TRUE;
    }
  
    mouse.dx += currX - mouse.x;
    mouse.dy += currY - mouse.y;
    mouse.x = currX;
    mouse.y = currY;
  // }

  return EM_TRUE;
}


EM_BOOL mouseButtonCallback(int eventType, const EmscriptenMouseEvent* e, void* userData){
  std::string button = "Mouse" + std::to_string(e->button);
  auto it = keyToAction.find(button);
  if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN){
    if (it != keyToAction.end()){
      actionPressed[it->second] = true;

      if (it->second == click){
        mouse.justClicked = true;

        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        long long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        mouse.clickTime = currentTime;
      }
      if (it->second == freeLook){
        mouse.dx = 0.0;
        mouse.dy = 0.0;
        mouse.first = true;
      }
    }
  }else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP){
    if (it != keyToAction.end()){
      actionPressed[it->second] = false;
    }
  }

  return EM_TRUE;
}


EM_BOOL mouseWheelCallback(int eventType, const EmscriptenWheelEvent* e, void* userData){
  std::string wheel;

  if (e->deltaY > 0){
    wheel = "MouseWheelUp";
  }else if (e->deltaY < 0){
    wheel = "MouseWheelDown";
  }

  auto it = keyToAction.find(wheel);
  if (it != keyToAction.end()){
    actionPressed[it->second] = true;
  }

  return EM_TRUE;
}

void updateMouse(){
  if (mouse.pointerLock){
    mouse.dx = 0.0;
    mouse.dy = 0.0;
  }

  if (actionPressed[freeLook]){
    mouse.pointerLock = true;
    hideMouse();
  }else {
    mouse.pointerLock = false;
    showMouse();
  }

  if (mouse.justClicked){
    mouse.justClicked = false;
  }

  if (actionPressed[zoomIn]){
    actionPressed[zoomIn] = false;
  }
  if (actionPressed[zoomOut]){
    actionPressed[zoomOut] = false;
  }

}

void setup(){
  preventPageScroll();

  emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,nullptr,EM_TRUE,keyCallback);
  emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,nullptr,EM_TRUE,keyCallback);
  emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,nullptr,EM_TRUE,mouseMoveCallback);
  emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,nullptr,EM_TRUE,mouseButtonCallback);
  emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,nullptr,EM_TRUE,mouseButtonCallback);
  emscripten_set_wheel_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,nullptr,EM_TRUE,mouseWheelCallback);
}

} // namespace input
