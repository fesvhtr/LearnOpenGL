#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 sky_projection;
uniform mat4 sky_view;

void main()
{
    TexCoords = aPos;
    gl_Position = sky_projection * sky_view * vec4(aPos, 1.0);
}