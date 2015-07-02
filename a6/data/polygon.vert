#version 150

in vec3 in_vertex;

uniform mat4 viewprojection;

void main()
{
    gl_Position = viewprojection * vec4(in_vertex, 1.0);
}
