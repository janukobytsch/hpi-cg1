/*
 * Aerialabs - Flow-based Abstraction of Aerial Images.
 * 08/23/2009 Amir Semmo
*/

#pragma once

#ifdef WIN32
#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#endif

#include <cmath>
#include <cassert>
#include <fstream>
#include <cfloat>
#include <algorithm>
#include <iostream>

#include <QtGui>
#include <QtOpenGL/QtOpenGL>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
