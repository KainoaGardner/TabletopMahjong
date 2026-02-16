#version 300 es
precision mediump float;

layout(location = 0) in vec2 aPos;

uniform vec2 uStart;
uniform vec2 uEnd;

out vec2 vTexCoord;

void main() {
    vTexCoord = aPos;

    vec2 minPos = min(uStart, uEnd);
    vec2 maxPos = max(uStart, uEnd);
  
    vec2 pos = mix(minPos, maxPos, aPos);

    gl_Position = vec4(pos, 0.0, 1.0);
}
