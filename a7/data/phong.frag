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
    ///////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 22
    // Implement phong shading.
    ///////////////////////////////////////////////////////////////////
    
    // write Total Color:
    vec4 resultColor = vec4(0.0, 0.0, 0.0, 1.0); // only placeholder color

    out_color = clamp(resultColor,0.0,1.0);
}
