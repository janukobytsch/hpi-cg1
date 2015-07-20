#pragma once

#include "openglfunctions.h"

struct LightingDefinition
{
    GLfloat m_pos[4];
    GLfloat m_iAmbient[4];
    GLfloat m_iDiffuse[4];
    GLfloat m_iSpecular[4];
};