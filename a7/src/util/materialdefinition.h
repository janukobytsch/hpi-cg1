#pragma once

#include "openglfunctions.h"

struct MaterialDefinition
{
    GLfloat m_kAmbient[4];
    GLfloat m_kDiffuse[4];
    GLfloat m_kSpecular[4];
    GLfloat m_kEmission[4];
    GLfloat m_shininess;
};