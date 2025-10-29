#version 300 es
precision mediump float;

out vec4 fragColor;

in vec2 vTexCoord;

uniform vec2 uTexOffset;
uniform sampler2D uDiffuse0;

void main() {
    vec2 uv = vTexCoord;
    uv.x += uTexOffset.y;
    uv.y += uTexOffset.x;

    vec4 color = texture(uDiffuse0, uv);

    fragColor = color;
}
