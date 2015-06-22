#pragma once

#include <cstring>
#include <sstream>

#include <QVector>

#include <QVector2D>
#include <QVector3D>

class Group;
class PolygonalDrawable;

class ObjIO
{
    typedef QVector<unsigned int> t_uints;

    typedef QVector<QVector3D> t_vec3s;
    typedef QVector<QVector2D> t_vec2s;

public:
    static PolygonalDrawable * fromObjFile(const std::string& filePath);

protected:

    struct ObjGroup
    {
        t_uints vis;
        t_uints vtis;
        t_uints vnis;

        // indices are stored in groups for, e.g., triangles, quads, polygons etc.
        // foffs stores the indices offsets to each face. This allows storing the
        // obj indices in the simplest and most supportive way.
        t_uints foffs;
    };

    typedef QVector<ObjGroup *> t_groups;

    struct ObjObject : public ObjGroup
    {
        t_vec3s vs;
        t_vec2s vts;
        t_vec3s vns;

        t_groups groups;

        ~ObjObject();
    };

    typedef QVector<ObjObject *> t_objects;

    enum e_FaceFormat
    {
        FF_V
    ,   FF_VT
    ,   FF_VN
    ,   FF_VTN
    };


protected:
    static PolygonalDrawable * createPolygonalDrawable(
        const ObjObject & object
    ,   const ObjGroup & group);

    static void parseV(
        std::istringstream & line
    ,   ObjObject & object);
    static void parseVT(
        std::istringstream & line
    ,   ObjObject & object);
    static void parseVN(
        std::istringstream & line
    ,   ObjObject & object);
    static void parseF(
        std::istringstream & line
    ,   ObjObject & object);
    static void parseO(
        std::istringstream & line
    ,   t_objects & objects);
    static void parseG(
        std::istringstream & line
    ,   ObjObject & object); 

    static const e_FaceFormat parseFaceFormat(const std::istringstream & line);
};