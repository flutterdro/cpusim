#version 330 core

out vec4 FragColor;

in vec2 FragPos;
in vec2 TexCoord;

uniform sampler2D tex;

void main() {
    FragColor = vec4(vec3(texture(tex, TexCoord).r), 1.0);
}
