#version 150

in vec3 in_vertex;
in vec3 in_normal;

out vec3 geom_normal;

void main()
{
    gl_Position = vec4(in_vertex, 1.0);
    geom_normal = in_normal;
}
