#pragma once

#include <QVector>

#include <QVector2D>
#include <QVector3D>

typedef QVector<unsigned int> t_uints;

typedef QVector<QVector3D> t_vec3s;
typedef QVector<QVector2D> t_vec2s;

class VertexReuse
{
public:
    static void optimize(
        t_vec3s & vertices
    ,   t_vec3s & normals
    ,   t_vec2s & texcs
    ,   t_uints & indices);
};
