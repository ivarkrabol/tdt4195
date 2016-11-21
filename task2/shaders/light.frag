#version 430 core

uniform mat4 model;
uniform vec3 lightDirection;

     in vec4 fragColor;
     in vec3 fragVert;
flat in vec3 fragNormal;

out vec4 color;

void main()
{
	vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
	vec3 pos = vec3(model * vec4(fragVert, 1));
    vec3 ambient = 0.4 * fragColor.rgb;
    vec3 diffuse = 0.6 * max(0.0, dot(normal, -normalize(lightDirection))) * fragColor.rgb;
    color = vec4(ambient + diffuse, fragColor.a);
}
