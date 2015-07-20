#pragma once

#include <QOpenGLFunctions_2_1>
#include <QOpenGLFunctions_3_2_Core>

class OpenGLFunctions : public QOpenGLFunctions_3_2_Core
{
};

class OpenGLFunctions21 : public QOpenGLFunctions_2_1
{
};
