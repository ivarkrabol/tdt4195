#version 430 core

uniform mat4 transform;

in vec4 position;
in vec4 acolor;

out vec4 vcolor;

void main()
{
    gl_Position = transform * position;
    vcolor = acolor;
}
