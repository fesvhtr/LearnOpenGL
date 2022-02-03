#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_window;

void main()
{

    FragColor = texture(texture_window, TexCoords);
}