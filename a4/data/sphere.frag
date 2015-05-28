#version 150

in vec4 vertex;
in vec3 normal;
in vec3 texCoord;

uniform sampler2D diffuse;

out vec4 out_color;

uniform vec3 lightsource;
uniform mat3 normalMatrix;

const float PI = 3.141592643589;

void main()
{
    float lambert = clamp(dot(normalize(normalMatrix * normalize(normal)), normalize(lightsource - vertex.xyz)), 0.0, 1.0);
    
    // See http://en.wikipedia.org/wiki/UV_mapping#Finding_UV_on_a_sphere
    vec2 coord = vec2(
	0.5 + atan(texCoord.z, texCoord.x) / (2.0 * PI),
	(0.5 - asin(texCoord.y) / PI)
    );
    
    vec4 color = texture(diffuse, coord);
    out_color = vec4((0.8 + 0.2 * lambert) * color.xyz, color.w);
}
