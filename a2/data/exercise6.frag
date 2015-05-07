#version 150

// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 2
//                     - Aufgabe 6
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

//////////////////////////////////////////////////
// TODO: Aufgabe 6d) - Interpolationsmodi anpassen
//
// Hinweis: Die Interpolationsmodi koennen vom laufenden Programm mittels der Tasten <1>, <2> und <3> gewechselt werden.
//////////////////////////////////////////////////

smooth in vec3 colorValue1;
flat in vec3 colorValue2;
noperspective in vec3 colorValue3;

out vec4 color;

uniform int interpolationMode;

void main()
{
    vec3 colorValue;

    switch (interpolationMode) {
        case 1:
            colorValue = colorValue1;
            break;
        case 2:
            colorValue = colorValue2;
            break;
        case 3:
            colorValue = colorValue3;
            break;
        default:
            colorValue = colorValue1;
            break;
    }

    color = vec4(colorValue, 1.0);
}
