#version 150

// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 7
//                     - Aufgabe 22 (Toon)
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

in vec3 normal;

out vec4 out_color;

uniform vec4 light_pos;

void main()
{   
    float intensity;
	vec4 baseColor = vec4(0.8, 0.1, 0.1, 1.0);
	vec4 color = baseColor;
	
	vec3 normN = normalize(normal);
    
    ///////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 22
    // Implement toon shading. Take the intensity and baseColor into account
    ///////////////////////////////////////////////////////////////////
        
	out_color = color;
}
