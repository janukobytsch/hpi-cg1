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
        float colorCoding = dot(normN, normalize(light_pos.xyz));
    
    ///////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 22
    // Implement toon shading. Take the intensity and baseColor into account
    ///////////////////////////////////////////////////////////////////

        if(colorCoding < 0.01){
            color = vec4(0.0, 0.0, 0.0, 1.0);
        }else if(colorCoding < 0.99){
            if(colorCoding < 0.34){
                color *= 0.2;
            } else if(colorCoding < 0.7){
                color *= 0.5;
            } else {
                color *= 0.9;
            }
        }else{
            color = vec4(1.0, 1.0, 1.0, 1.0);
        }
        
	out_color = color;
}
