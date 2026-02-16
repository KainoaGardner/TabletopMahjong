#version 300 es
precision mediump float;

out vec4 fragColor;
in vec2 vTexCoord;

uniform sampler2D uTexture0;

void main() {
    vec4 color = texture(uTexture0, vTexCoord);
    fragColor = vec4(color);
}
