#version 410 core

in vec2 texCoords;

uniform sampler2D tex;

out vec4 fragColor;

void main(void) {
    fragColor = texture(tex, texCoords);
}