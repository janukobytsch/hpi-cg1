// Copyright (c) 2012-2013, Computer Graphics Systems Group at the
// Hasso-Plattner-Institute, Germany. All rights reserved.

#pragma once

#include <cmath>


#define _PI 3.1415926535897932384626433832795L
#define _PI2  (_PI * 2.00L)
#define _PI_2 (_PI * 0.50L)
#define _PI4  (_PI * 4.00L)
#define _PI_4 (_PI * 0.25L)

#define _abs(v) \
    (((v) < 0) ? -(v) : (v))

#define _int(v) \
    (static_cast<int>(v))

#define _short(v) \
    (static_cast<short>(v))

#define _mod(a, m) \
    ((a) - (m) * (_int((a) / (m)) - ((a) < 0 ? 1 : 0)))

#define _frac(x) \
    ((x) - _int(x))

#define _clamp(l, u, x) \
    ((x) < (l) ? (l) : (x) > (u) ? (u) : (x))

#define _deg(rad) \
    ((rad) * 180.0L / _PI)

#define _rad(deg) \
    ((deg) * _PI / 180.0L)

#define _mi(a, b) \
    ((a) < (b) ? (a) : (b))

#define _ma(a, b) \
    ((a) < (b) ? (b) : (a))

// same as _hour
// note: if d is negative use -_decimal(d, m, s) instead of _decimal(-d, m, s)
#define _decimal(d, m, s) \
    ((d) + ((m) + (s) / 60.0L) / 60.0L)

#define _sind(deg) \
    (sin(_rad(deg)))

#define _cosd(deg) \
    (cos(_rad(deg)))

#define _tand(deg) \
    (tan(_rad(deg)))

#define _asind(rad) \
    (_deg(asin(rad)))

#define _acosd(rad) \
    (_deg(acos(rad)))

#define _atand(rad) \
    (_deg(atan(rad)))

#define _atan2d(x, y) \
    (_deg(atan2(x, y)))

#define _adiameter(D, r) \
    (2 * atan(r / D))

// normalizes an angle to between 0 and 2PI radians
#define _rev(rad) \
    ((rad) - floor((rad) / _PI2) * _PI2)

// normalizes an angle to between 0 and 360 degrees
#define _revd(deg) \
    ((deg) - floor((deg) / 360.0L) * 360.0L)

// cube root (e.g. needed for parabolic orbits)
#define _cbrt(x) \
    (((x) > 0.0) ? exp(log(x) / 3.0L) : (((x) < 0.0) ? -cbrt(-(x)) : 0.0))


#define __b02(x) (     (x) | (     (x) >>  1))
#define __b04(x) (__b02(x) | (__b02(x) >>  2))
#define __b08(x) (__b04(x) | (__b04(x) >>  4))
#define __b16(x) (__b08(x) | (__b08(x) >>  8))
#define __b32(x) (__b16(x) | (__b16(x) >> 16))

// Returns the next power of an integer.
#define _nextPowerOf2(x) \
    (__b32((x) - 1) + 1)

#define _prevPowerOf2(x) \
    (_nextPowerOf2(x) >> 1)

#define _randf(min, max) \
    (static_cast<float>(rand()) / RAND_MAX * ((max) - (min)) + (min))

#define _rand(min, max) \
    (static_cast<int>(static_cast<float>(rand()) / RAND_MAX * ((max) - (min)) + (min)))
