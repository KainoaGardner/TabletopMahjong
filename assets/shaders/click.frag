#version 300 es
precision mediump float;

out vec4 fragColor;

uniform vec3 uColor;

void main() {
    fragColor = vec4(uColor, 0.5);
}
