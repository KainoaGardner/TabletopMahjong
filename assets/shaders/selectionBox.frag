#version 300 es
precision mediump float;

out vec4 fragColor;

in vec2 vTexCoord;

uniform vec4 uColor;
uniform vec2 uThickness;

float getEdge(vec2 pos, vec2 thickness){
  vec2 result = step(thickness, pos) * step(pos, 1.0 - thickness);
  return result.x * result.y;
}

void main() {
    vec2 uv = vTexCoord;

    vec2 thickness = fwidth(uv) * uThickness;

    float edge = getEdge(uv, thickness);
    float alpha = mix(1.0, 0.1, edge);

    fragColor = vec4(uColor.xyz, alpha);
}
