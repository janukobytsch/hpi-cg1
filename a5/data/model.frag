#version 150

in vec4 vertex;
in vec3 normal;

out vec4 out_color;

uniform vec3 lightsource;
uniform mat3 normalMatrix;
uniform vec4 color;

void main()
{
    float lambert = clamp(dot(normalize(normalMatrix * normalize(normal)), normalize(lightsource - vertex.xyz)), 0.0, 1.0);
    out_color = vec4((0.2 + 0.8 * lambert) * color.xyz, color.w);
}
