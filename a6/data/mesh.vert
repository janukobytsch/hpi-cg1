#version 150

uniform mat4 viewprojection;

in vec3 in_vertex;
in vec3 in_normal;

out vec3 normal;
out vec4 vertex;

void main()
{
    vec4 v = viewprojection * vec4(in_vertex, 1.0);

    vertex = v;
    gl_Position = v;
    
    normal = in_normal;
}
