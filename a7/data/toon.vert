#version 150

in vec3 in_vertex;
in vec3 in_normal;

uniform mat4 viewprojection;
uniform mat3 normalMatrix;

out vec3 normal;

void main()
{
    normal = normalize(normalMatrix * in_normal);
    
    gl_Position = viewprojection * vec4(in_vertex, 1.0);
}
