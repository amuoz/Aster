#version 330 core
out vec4 Color;

in vec2 TexCoords;

uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{
    Color = vec4(spriteColor) * texture(image, TexCoords);
}