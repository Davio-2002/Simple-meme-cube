#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Color;

uniform sampler2D uTexture;
uniform sampler2D uTexture2;
uniform float mixValue;

void main() {
    vec4 image_1 = texture(uTexture, TexCoord);
    vec4 image_2 = texture(uTexture2, TexCoord);
    FragColor = mix(image_1, image_2, mixValue);
}
