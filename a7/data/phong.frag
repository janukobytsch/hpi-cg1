#version 150

// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 7
//                     - Aufgabe 22 (Phong)
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

in vec3 normal;
in vec4 vertex;

out vec4 out_color;

uniform vec4 light_pos;
uniform vec4 light_iAmbient;
uniform vec4 light_iDiffuse;
uniform vec4 light_iSpecular;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform vec4 material_emission;
uniform float material_shininess;

void main()
{
    // Implement phong shading
    vec4 ambient, diffuse, specular, phongColor;

    vec3 surfaceNormal = normalize(normal);
    vec3 lightDir = normalize(light_pos.xyz);
    vec3 viewDir = normalize(vertex.xyz);
    vec3 reflectionDir = normalize(2 * dot(surfaceNormal, lightDir) * surfaceNormal - lightDir);

    float diffuseWeight = max(dot(surfaceNormal, lightDir), 0);
    float specularWeight = pow(max(dot(reflectionDir, viewDir), 0), 64*material_shininess);

    for (int i=0; i<4; i++)
    {
        ambient[i] = light_iAmbient[i] * material_ambient[i];
        diffuse[i] = light_iDiffuse[i] * material_diffuse[i] * diffuseWeight;
        specular[i] = light_iSpecular[i] * material_specular[i] * specularWeight;
        // Attenuation and emission are not considered
        phongColor[i] = ambient[i] + diffuse[i] + specular[i];
    }

    out_color = clamp(phongColor, 0.0, 1.0);
}
