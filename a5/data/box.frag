#version 150

in vec3 normal;
in vec4 vertex;

out vec4 out_color;

uniform vec3 lightsource;
uniform mat3 normalMatrix;
uniform vec4 color;
uniform bool useLighting;

void main()
{
	if(useLighting)
	{
		float lambert = clamp(dot(normalize(normalMatrix * normalize(normal)), normalize(lightsource - vertex.xyz)), 0.0, 1.0);
		out_color = vec4(0.4 * color.xyz + color.xyz * 0.6 * lambert, color.w);
	}
	else
		out_color = vec4(0.0, 0.0, 0.0, 1.0);
}
