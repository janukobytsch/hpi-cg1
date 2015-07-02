
#include <cstring>
#include <iostream>
#include <fstream>
#include <map>

#include <QtOpenGL>
#include <QFile>

#include "objio.h"
#include "polygonaldrawable.h"


using namespace std;

// http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

// trim from start
static inline void trim(std::string & str) 
{
    // trim trailing spaces
    const size_t endpos = str.find_last_not_of(" \t");
    if(string::npos != endpos)
        str.substr(0, endpos + 1).swap(str);

    // trim leading spaces
    const size_t startpos = str.find_first_not_of(" \t");
    if(string::npos != startpos)
        str.substr(startpos).swap(str);
}

ObjIO::ObjObject::~ObjObject()
{
    groups.clear();
}

PolygonalDrawable * ObjIO::fromObjFile(const std::string& filePath)
{
    // http://en.wikipedia.org/wiki/Wavefront_.obj_file
    // http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ

	const QString filePathQ = QString::fromStdString(filePath);

    if(!QFile::exists(filePathQ))
    {
        qDebug("Reading geometry failed: \"%s\" does not exist.", qPrintable(filePathQ));
        return NULL;
    }

    qDebug("Reading geometry from \"%s\".", qPrintable(filePathQ));

    ifstream stream(filePath.c_str(), ios::in);
    if(!stream)
    {
        //qCritical("Read from \"%s\" failed.", path.c_str());
        return NULL;
    }

    t_objects objects;

    string line;
    while(getline(stream, line))
    {
        if(line.empty() || line.length() < 2)
            continue;

        const string type(line.substr(0, 2));
        istringstream s(line.substr(2));

        if("o " == type)
        {
            parseO (s, objects);
            continue;
        }
        if("# " == type)
            continue;

        if("v " != type && "vt" != type && "vn" != type && "f " != type && "g " != type)
            continue;


        // at this point an object is required!

        if(objects.empty())
            objects.push_back(new ObjObject);

        // always work on the last object
        ObjObject & object(*objects.back());

        if     ("v " == type)
            parseV (s, object);
        else if("vt" == type)
            parseVT(s, object);
        else if("vn" == type)
            parseVN(s, object);
        else if("f " == type)
            parseF (s, object);
        else if("g " == type)
            parseG (s, object);
    }
    stream.close();

    ObjObject & object(*objects.first());

    // TODO: return all objects... no only the first
    return createPolygonalDrawable(object, object);
}

inline void ObjIO::parseV(
    istringstream & line
,   ObjObject & object)
{
    float x, y, z;

    line >> x;
    line >> y;
    line >> z;

    QVector3D v(x, y, z);

    float w(1.f);
    if(line >> w)
        v /= w;

    object.vs.push_back(v);
}

inline void ObjIO::parseVT(
    istringstream & line
,   ObjObject & object)
{
    float s, t;

    line >> s;
    line >> t;

    QVector2D vt(s, t);

    float w(1.f);
    if(line >> w)
        vt /= w;

    object.vts.push_back(vt);
}

inline void ObjIO::parseVN(
    istringstream & line
,   ObjObject & object)
{
    float x, y, z;

    line >> x;
    line >> y;
    line >> z;

    QVector3D vn(x, y, z);

    object.vns.push_back(vn);
}

inline const ObjIO::e_FaceFormat ObjIO::parseFaceFormat(const istringstream & line)
{
    string s(line.str());
    trim(s);

    size_t l(s.find(" "));
    if(string::npos == l)
        l = s.length();

    const size_t f0(s.find("/", 0));

    if(string::npos == f0)
        return FF_V;

    const size_t f1(s.find("/", f0 + 1));

    if(string::npos == f1 || f1 > l)
        return FF_VT;

    if(string::npos == s.find("//", 0))
        return FF_VTN;
    else
        return FF_VN;
}

inline void ObjIO::parseF(
    istringstream & line
,   ObjObject & object)
{
    ObjGroup & group = object.groups.empty() ? object : *object.groups.back();

    const e_FaceFormat format(parseFaceFormat(line));

    group.foffs.push_back(static_cast<GLuint>(group.vis.size()));

    GLuint i;

    while(line >> i)
    {
        group.vis.push_back(--i);

        switch(format)
        {
        case FF_V:  // v0 v1 ...

            break;

        case FF_VT: // v0/vt0 v1/vt1 ...

            line.ignore(1); // ignore slash
            line >> i;
            group.vtis.push_back(--i);

            break;
             
        case FF_VN: // v0//vn0 v1//vn1 ...

            line.ignore(2); // ignore slashes
            line >> i;
            group.vnis.push_back(--i);

            break;

        case FF_VTN: // v0/vt0/vn0 v1/vt1/vn1 ...

            line.ignore(1); // ignore slash
            line >> i;
            group.vtis.push_back(--i);

            line.ignore(1); // ignore slash
            line >> i;
            group.vnis.push_back(--i);

        default:
            break;
        }
    }
}

inline void ObjIO::parseO(
    istringstream & line
,   ObjIO::t_objects & objects)
{
    ObjObject * object(new ObjObject);
    //line >> object->name;

    objects.push_back(object);
}

inline void ObjIO::parseG(
    istringstream & line
,   ObjObject & object)
{
    ObjGroup * group(new ObjGroup);
    //line >> group->name;

    object.groups.push_back(group);
}

PolygonalDrawable * ObjIO::createPolygonalDrawable(
    const ObjObject & object
,   const ObjGroup & group)
{
    if(group.vis.empty())
        return NULL;

    // TODO: this should test if all consecutive offsets are equal 3.
    // The current expression could return false positives.
    if(group.vis.size() / 3 != group.foffs.size())
    {
        qCritical("Ignore polygon with num vertices != 3 (only triangles are supported).");
        return NULL;
    }

    const bool usesTexCoordIndices(!group.vtis.empty());
    const bool usesNormalIndices(!group.vnis.empty());

    PolygonalDrawable * drawable = new PolygonalDrawable();

    const GLuint size(static_cast<GLuint>(group.vis.size()));

    for(GLuint i = 0; i < size; ++i)
    {
        // add vertex and its new index based on obj index

		// TODO: make use of vertex reuse!

        drawable->indices().push_back(i);
        drawable->vertices().push_back(object.vs[group.vis[i]]);

        if(usesTexCoordIndices)
            drawable->texcs().push_back(object.vts[group.vtis[i]]);
        if(usesNormalIndices)
            drawable->normals().push_back(object.vns[group.vnis[i]]);
    }

    // TODO: support other modes here!
    drawable->setMode(GL_TRIANGLES);

    if(!usesNormalIndices)
        drawable->retrieveNormals();

    return drawable;
}
