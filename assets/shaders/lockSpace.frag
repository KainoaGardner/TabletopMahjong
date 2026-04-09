#version 300 es
precision mediump float;

out vec4 fragColor;
in vec2 vTexCoord;

uniform vec4 uColor;
uniform vec4 uBorderColor;
uniform vec2 uThickness;

uniform float uMaxFov;
uniform float uFov;

float getEdge(vec2 pos, vec2 thickness){
  vec2 result = step(thickness, pos) * step(pos, 1.0 - thickness);
  return result.x * result.y;
}

void main() {
    vec2 uv = vTexCoord;

    vec2 thickness = fwidth(uv) * uThickness;

    float normFov = uFov / uMaxFov;
    thickness /= normFov;

    float edge = getEdge(uv, thickness);
    vec4 color = mix(uColor, uBorderColor, edge);

    fragColor = color;
}
