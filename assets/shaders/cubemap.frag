#version 300 es
precision mediump float;

out vec4 fragColor;
in vec3 vTexCoord;

uniform samplerCube uTexture0;

void main() {
    vec4 color = texture(uTexture0, vTexCoord);
    fragColor = color;
}
