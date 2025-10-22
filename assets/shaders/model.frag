#version 300 es
precision mediump float;

out vec4 fragColor;

in vec2 vTexCoord;

uniform sampler2D uDiffuse0;

void main() {
    vec4 color = texture(uDiffuse0, vTexCoord);
    fragColor = color;
}
