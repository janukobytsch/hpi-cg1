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

in float colorValue1;
in float colorValue2;
in float colorValue3;

out vec4 color;

uniform int interpolationMode;

void main()
{
    // TODO: Farbwert auslesen (vom Modus abhaengig)
    float colorValue = 0.0;
    
    //////////////////////////////////////////////////
    // TODO: Aufgabe 6c) - Farbe berechnen
    //////////////////////////////////////////////////

    // Als Graustufenwert interpretieren
    color = vec4(colorValue, colorValue, colorValue, 1.0);

    // TODO: Interpoliert
}
