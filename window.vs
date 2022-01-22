#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 window_trans;
out vec2 TexCoords;

void main()
{
    gl_Position = window_trans * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}