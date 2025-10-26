#version 300 es
precision mediump float;

out vec4 fragColor;

in vec2 vTexCoord;

uniform vec2 uTexScale;
uniform sampler2D uDiff;

void main() {
    vec2 uv = vTexCoord;
    uv *= uTexScale;

    vec3 color = texture(uDiff, uv).rgb;
    fragColor = vec4(color,1.0);
}
