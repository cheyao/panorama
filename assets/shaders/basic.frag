#version 400 core
precision mediump float;

in vec2 texPos;

uniform sampler2D box;
uniform sampler2D face;

out vec4 color;

void main() {
    color = mix(texture(box, texPos), texture(face, texPos), 0.2f);
} 
