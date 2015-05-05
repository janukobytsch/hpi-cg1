#pragma once

#include "util/painter.h"
#include "util/triangle.h"

class QTimer;

class Exercise6 : public QObject, public Painter
{
    Q_OBJECT

public:
    Exercise6();
    virtual ~Exercise6();

    virtual bool initialize();

protected:
    void transferGeometryToGPU();
    void tessellate(const Triangle &triangle, std::vector<Triangle> &triangleBuffer);

    virtual void render() override;

    void drawTriangles(int start, int count);

protected slots:
    /**
    *  @brief
    *    Called when the timer has fired
    */
    void onTimer();

protected:
    std::vector<Triangle> m_triangles;
    std::vector<Triangle> m_trianglesRefined;
    std::vector<Triangle>* m_currentBuffer;
    std::vector<Triangle>* m_helperBuffer;
    bool m_update;
    int m_tessellationDepth;
    QTimer * m_timer;        /**< Timer */
};
