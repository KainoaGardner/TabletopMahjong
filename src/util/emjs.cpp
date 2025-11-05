#include <emscripten.h>

EM_JS(void, hideMouse, (), {
  const canvas = document.getElementById("canvas");
  if (!canvas) return;

  canvas.style.cursor = "none";
});

EM_JS(void, showMouse, (), {
  const canvas = document.getElementById("canvas");
  if (!canvas) return;

  canvas.style.cursor = "pointer";
});


EM_JS(double, getCanvasMouseX, (int clientX), {
    const canvas = Module['canvas'] || document.getElementById('canvas');
    if (!canvas) return 0;
    const rect = canvas.getBoundingClientRect();
    return clientX - rect.left;
});

EM_JS(double, getCanvasMouseY, (int clientY), {
    const canvas = Module['canvas'] || document.getElementById('canvas');
    if (!canvas) return 0;
    const rect = canvas.getBoundingClientRect();
    return clientY - rect.top;
});


EM_JS(void, preventPageScroll, (), {
    document.addEventListener('wheel', e => e.preventDefault(), { passive: false });
    document.addEventListener('touchmove', e => e.preventDefault(), { passive: false });
});
