#version 150

// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 5
//                     - Aufgabe 18
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: 770755
// Matr.-Nr: xxxxx
//
// ======================================

in vec4 vertex;
in vec3 normal;

out vec4 out_color;

uniform vec3 lightsource;
uniform mat3 normalMatrix;
uniform vec4 color;

void main()
{
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 18, part 5
    // Adjust the color depending on the depth of the vertex.
    /////////////////////////////////////////////////////////////////////////////////////////////////

    // Hacky workaround because the distribution of the values is so small
    float z = 1 -((gl_FragCoord.z - 0.95) * 20);

    out_color = vec4(z, z, z, 1);
}
