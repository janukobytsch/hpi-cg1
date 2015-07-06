
#pragma once

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "util/abstractexercise.h"

#include <vector>
#include <list>

class PolygonalDrawable;

struct Vertex
{
    double x;
    double y;
    double z;

    Vertex(
        const double x
    ,   const double y
    ,   const double z)
    :   x(x)
    ,   y(y)
    ,   z(z)
    {
    }
};

typedef std::vector<Vertex>  Contour;
typedef std::vector<Contour> ContourList;

class Exercise19 : public AbstractExercise, protected OpenGLFunctions21
{
public:
    Exercise19();
    virtual ~Exercise19();

    // from abstractglexercise.h
    virtual void render();

    // from abstractexercise.h
    virtual bool initialize();

    virtual void resize(int width, int height) override;

    virtual const QString hints() const;
    virtual const QString description() const;

    virtual bool onMouseReleased(QMouseEvent * mouseEvent);
    virtual bool onKeyPressed(QKeyEvent * event);


protected:
    void drawContours();
    void tessellatePolygons();

    ContourList m_contours;
    GLUtesselator* m_tesselator;
};
