#version 150

#define M_PI 3.1415926535897932384626433832795

// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 5
//                     - Aufgabe 17
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

uniform mat4 transform;
uniform mat4 viewprojection;
uniform vec3 overallObjectDimensions;
uniform float animationFrame;
uniform int globalDeformationMode;

in vec3 in_vertex;
in vec3 in_normal;

out vec3 normal;
out vec4 vertex;

vec3 mold(vec3 v, float moldPlateau)
{
    //////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 17, part 1
    // Apply a mold deformation to the given vertex v.
    // Take into account the moldPlateau parameter
    //      1.0f: No deformation at all
    //      0.0f: Maximum deformation
    // Tip: Use overallObjectDimensions to get the extents of the x, y and z dimension
    // Tip: Keep in mind that the box is located in the coordinate system origin
    /////////////////////////////////////////////////////////////////////////////////////////////////

    float angle = abs(atan(v.x/v.z));

    vec4 vTransformationRepresantation = vec4(v.x, v.y, v.z, 1);

    mat4 transformation = mat4(1,0,0,0,0,1,0,0,0,0,1.0 - ((1.0 - moldPlateau) * angle),0,0,0,0,1);

    vec4 nV = vTransformationRepresantation * transformation;

    v = nV.xyz;

    return v;
}

vec3 pinch(vec3 v, float pinchPlateau)
{
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 17, part 2
    // Apply a pinch deformation to the given vertex v.
    // Take into account the pinchPlateau parameter
    //      1.0f: No deformation at all
    //      0.0f: Maximum deformation
    // Tip: Use overallObjectDimensions to get the extents of the x, y and z dimension
    // Tip: Keep in mind that the box is located in the coordinate system origin
    /////////////////////////////////////////////////////////////////////////////////////////////////

    float scaleFactor = ( 1.0 - (pinchPlateau * ((v.y + (overallObjectDimensions.y/2))/overallObjectDimensions.y)));

    vec4 vTransformationRepresantation = vec4(v.x, v.y, v.z, 1);

    mat4 transformation = mat4(scaleFactor,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

    vec4 nV = vTransformationRepresantation * transformation;

    v = nV.xyz;

    return v;
}

vec3 twist(vec3 v, float maxAngle)
{
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 17, part 3
    // Apply a twist deformation to the given vertex v.
    // Take into account the maxAngle parameter, that defines the maximum rotation angle
    // Tip: Use overallObjectDimensions to get the extents of the x, y and z dimension
    // Tip: Keep in mind that the box is located in the coordinate system origin
    /////////////////////////////////////////////////////////////////////////////////////////////////

    float heightFactor = (v.y + (overallObjectDimensions.y/2))/overallObjectDimensions.y;

    float rotation = heightFactor * maxAngle;

    vec4 vTransformationRepresantation = vec4(v.x, v.y, v.z, 1);

    mat4 transformation = mat4(cos(rotation),0,-sin(rotation),0,0,1,0,0,sin(rotation),0,cos(rotation),0,0,0,0,1);

    vec4 nV = vTransformationRepresantation * transformation;

    v = nV.xyz;

    return v;
}


vec3 bend(vec3 v, float maxAngle)
{
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 17, part 4
    // Apply a bend deformation to the given vertex v.
    // Take into account the maxAngle parameter, that defines the maximum rotation angle
    // Tip: Use overallObjectDimensions to get the extents of the x, y and z dimension
    // Tip: Keep in mind that the box is located in the coordinate system origin
    /////////////////////////////////////////////////////////////////////////////////////////////////

    float heightFactor = (v.y + (overallObjectDimensions.y/2))/overallObjectDimensions.y;

    float rotation = heightFactor * maxAngle;

    vec4 vTransformationRepresantation = vec4(v.x, v.y, v.z, 1);

    mat4 transformation = mat4(cos(rotation),sin(rotation),0,0,-sin(rotation),cos(rotation),0,0,0,0,1,0,0,0,0,1);

    vec4 nV = vTransformationRepresantation * transformation;

    v = nV.xyz;

    return v;
}

void main()
{
    vec3 transV;

    if(globalDeformationMode == 1)
        transV = mold(in_vertex, 1.0f - animationFrame);
    else if(globalDeformationMode == 2)
        transV = pinch(in_vertex, 1.0f - animationFrame);
    else if(globalDeformationMode == 3)
        transV = twist(in_vertex, radians(animationFrame * 360.0f));
    else if(globalDeformationMode == 4)
        transV = bend(in_vertex, radians(animationFrame * 90.0f));
    else
        transV = in_vertex;

    gl_Position = viewprojection * transform * vec4(transV, 1.0);
    vertex = transform * vec4(transV, 1.0);
    normal = in_normal;
}
