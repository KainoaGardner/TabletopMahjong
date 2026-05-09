#version 300 es
precision mediump float;

out vec4 fragColor;

in vec2 vTexCoord;

uniform vec2 uTexOffset;
uniform sampler2D uDiffuse0;
uniform sampler2D uNumTex;

uniform float uNums;

// const vec4 numColor = vec4(1.0, 0.0, 0.0, 1.0);
const vec4 numColor = vec4(0.75, 0.22, 0.17, 1.0);

void main() {
    vec2 uv = vTexCoord;
    uv.x += uTexOffset.y;
    uv.y += uTexOffset.x;

    vec4 color = texture(uDiffuse0, uv);
    float num = texture(uNumTex, uv).a * uNums;

    vec4 finalColor = mix(color, numColor, num);
    fragColor = finalColor;
}
