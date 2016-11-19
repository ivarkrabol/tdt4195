#version 430 core

in vec3 position;
in vec4 acolor;
out vec4 vcolor;

void main()
{
    gl_Position = vec4(position, 1.0f);
    vcolor = acolor;
}
