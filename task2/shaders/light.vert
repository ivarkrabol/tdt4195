#version 430 core

uniform mat4 camera;
uniform mat4 model;

in vec3 vert;
in vec4 vertColor;
in vec3 vertNormal;

     out vec4 fragColor;
     out vec3 fragVert;
flat out vec3 fragNormal;

void main()
{
    fragColor  = vertColor;
    fragVert   = vert;
    fragNormal = vertNormal;

    gl_Position = camera * model * vec4(vert, 1);
}
