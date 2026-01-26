#include "../include/engine/input.hpp"
#include "../include/util/emjs.hpp"
#include <chrono>


namespace input {

std::unordered_map<std::string, actions>keyToAction = {
    {"KeyW", forward},
    {"KeyS", backward},
    {"KeyA", left},
    {"KeyD", right},
    {"KeyQ", up},
    {"KeyE", down},
    {"Mouse0", click},
    {"Mouse2", freeLook},
    {"KeyF", freeCam},
    {"MouseWheelUp", zoomIn},
    {"MouseWheelDown", zoomOut},
    {"KeyR", revertCamera},
    {"KeyP", perspective},
    {"Digit1", mainCamera},
    {"Digit2", pointCamera},
    {"Digit3", topCamera},

    //remove later
    {"Digit6", camera1},
    {"Digit7", camera2},
    {"Digit8", camera3},
    {"Digit9", camera4},
};

} // namespace input


Mouse Input::mouse{};
std::unordered_map<input::actions, bool> Input::actionPrev{};
std::unordered_map<input::actions, bool> Input::actionCurr{};

EM_BOOL Input::keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData){
  auto it = input::keyToAction.find(e->code);
  if (eventType == EMSCRIPTEN_EVENT_KEYDOWN){
    if (it != input::keyToAction.end()){
      actionCurr[it->second] = true;
    }

    if (it->second == input::click){
      mouse.justClicked = true;

      auto now = std::chrono::system_clock::now();
      auto duration = now.time_since_epoch();
      long long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
      mouse.clickTime = currentTime;
    }
    if (it->second == input::freeLook){
      mouse.dx = 0.0;
      mouse.dy = 0.0;
      mouse.first = true;
    }


  }else if (eventType == EMSCRIPTEN_EVENT_KEYUP){
    if (it != input::keyToAction.end()){
      actionCurr[it->second] = false;
    }
  }

  return EM_TRUE;
}


EM_BOOL Input::mouseMoveCallback(int eventType, const EmscriptenMouseEvent* e, void* userData){
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


EM_BOOL Input::mouseButtonCallback(int eventType, const EmscriptenMouseEvent* e, void* userData){
  std::string button = "Mouse" + std::to_string(e->button);
  auto it = input::keyToAction.find(button);
  if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN){
    if (it != input::keyToAction.end()){
      actionCurr[it->second] = true;

      if (it->second == input::click){
        mouse.justClicked = true;

        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        long long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        mouse.clickTime = currentTime;
      }
      if (it->second == input::freeLook){
        mouse.dx = 0.0;
        mouse.dy = 0.0;
        mouse.first = true;
      }
    }
  }else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP){
    if (it != input::keyToAction.end()){
      actionCurr[it->second] = false;
    }
  }

  return EM_TRUE;
}


EM_BOOL Input::mouseWheelCallback(int eventType, const EmscriptenWheelEvent* e, void* userData){
  std::string wheel;

  if (e->deltaY > 0){
    wheel = "MouseWheelUp";
  }else if (e->deltaY < 0){
    wheel = "MouseWheelDown";
  }

  auto it = input::keyToAction.find(wheel);
  if (it != input::keyToAction.end()){
    actionCurr[it->second] = true;
  }


  return EM_TRUE;
}

void Mouse::update(std::unordered_map<input::actions, bool> actionCurr){
  if (!pointerLock){
    dx = 0.0;
    dy = 0.0;
  }

  if (actionCurr[input::freeLook]){
    pointerLock = true;
    // hideMouse();
  }else {
    pointerLock = false;
    // showMouse();
  }

  if (justClicked){
    justClicked = false;
  }

  if (actionCurr[input::zoomIn]){
    actionCurr[input::zoomIn] = false;
  }
  if (actionCurr[input::zoomOut]){
    actionCurr[input::zoomOut] = false;
  }

}

bool Input::pressed(input::actions a) const{
  return actionCurr[a];
}

bool Input::justPressed(input::actions a) const{

  return actionCurr[a] && !actionPrev[a];
}

bool Input::justReleased(input::actions a) const{
  return !actionCurr[a] && actionPrev[a];
}

void Input::clear(){
  // for (const auto& pair : actionCurr){
  //   actionCurr[pair.first] = false;
  // }

  actionCurr[input::actions::zoomIn] = false;
  actionCurr[input::actions::zoomOut] = false;

  mouse.dx = 0.0f;
  mouse.dy = 0.0f;
}


void Input::update(){
  mouse.update(actionCurr);
}

void Input::setup(){
  preventPageScroll();

  actionPrev = {
    {input::forward, false},
    {input::backward, false},
    {input::left, false},
    {input::right, false},     
    {input::up, false},
    {input::down, false},     
  };

  actionCurr = {
    {input::forward, false},
    {input::backward, false},
    {input::left, false},
    {input::right, false},     
    {input::up, false},
    {input::down, false},     
  };

  emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_TRUE, keyCallback);
  emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,nullptr,EM_TRUE,keyCallback);
  emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,nullptr,EM_TRUE,mouseMoveCallback);
  emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,nullptr,EM_TRUE,mouseButtonCallback);
  emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,nullptr,EM_TRUE,mouseButtonCallback);
  emscripten_set_wheel_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,nullptr,EM_TRUE,mouseWheelCallback);
}



