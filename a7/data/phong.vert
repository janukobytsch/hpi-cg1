#version 150

in vec3 in_vertex;
in vec3 in_normal;

uniform mat4 view;
uniform mat4 viewprojection;
uniform mat3 normalMatrix;

out vec3 normal;
out vec4 vertex;

void main()
{
    vertex = view * vec4(in_vertex, 1.0);
    normal = normalize(normalMatrix * in_normal);
    
    gl_Position = viewprojection * vec4(in_vertex, 1.0);
}
