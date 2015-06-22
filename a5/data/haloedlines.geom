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
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

layout (triangles) in;

/////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: Aufgabe 18, part 3
// Adjust the primitive type and number of max_vertices to create lines.
// Tip: Use primitive type "line_strip"
/////////////////////////////////////////////////////////////////////////////////////////////////
layout (line_strip, max_vertices = 4) out;

uniform mat4 viewprojection;
uniform float animationFrame;

in vec3 geom_normal[];

out vec4 vertex;
out vec3 normal;

void main()
{
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 18, part 4
    // Adjust the creation of the new vertices to create lines.
    /////////////////////////////////////////////////////////////////////////////////////////////////
    for (int i=0; i < 3; ++i)
    {
        normal = normalize(geom_normal[i]);
        vertex = gl_in[i].gl_Position;
        gl_Position = viewprojection * vertex;

        EmitVertex();
    }

    normal = normalize(geom_normal[0]);
    vertex = gl_in[0].gl_Position;
    gl_Position = viewprojection * vertex;

    EmitVertex();

    EndPrimitive();
}
