#version 300 es
precision mediump float;

out vec4 fragColor;

in vec2 vTexCoord;

uniform sampler2D uDiff;

void main() {
    vec3 color = texture(uDiff, vTexCoord).rgb;
    fragColor = vec4(color,1.0);
}
