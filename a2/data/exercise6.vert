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

uniform mat4 transform;
uniform sampler2D heightMap;

in vec2 position;

smooth out vec3 colorValue1;
flat out vec3 colorValue2;
noperspective out vec3 colorValue3;

const int colorCount = 5;
const vec3 colors[5] = vec3[5](
    vec3(0.400, 1, 0.2),
    vec3(0.204, 0.529, 0),
    vec3(0.22, 0.173, 0.039),
    vec3(0.502, 0.502, 0.502),
    vec3(0.914, 0.914, 0.914)
);

const float breakpoints[5] = float[5](0.0, 0.1, 0.33, 0.67, 0.9);

float getHeight(vec2 uv)
{
    return texture2D(heightMap, uv).r;
}

void clampColor(inout vec3 startColor, inout vec3 endColor, in float height, out float weight)
{
    startColor = colors[0];
    endColor = colors[colorCount-1];

    for (int i = 0; i < colorCount; ++i) {
        if (height >= breakpoints[i]) {
            startColor = colors[i];
            endColor = (i < colorCount-1) ? colors[i+1] : colors[i];
            float upperBreak = (i < colorCount-1) ? breakpoints[i+1] : 1.0;
            float lowerBreak = (i > 0) ? breakpoints[i] : 0.0f;
            weight = 1 - ((upperBreak - height) / (upperBreak - lowerBreak));
        }
    }

    weight = clamp(weight, 0.0, 1.0);
}

void main()
{
    float height = getHeight(position);

    // Calculate world coordinates
    vec3 worldCoord = vec3(position.x, position.y, height);
    gl_Position = transform * vec4(worldCoord.x, 3.0 * worldCoord.y - 0.25, worldCoord.z, 1.0);

    // Assign color values
    vec3 startColor;
    vec3 endColor;
    float weight = 1;

    clampColor(startColor, endColor, height, weight);
    vec3 colorValue = mix(startColor, endColor, weight);

    colorValue1 = colorValue;
    colorValue2 = colorValue;
    colorValue3 = colorValue;
}
