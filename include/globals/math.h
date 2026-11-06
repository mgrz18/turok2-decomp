#ifndef GLOBALS_MATH_H
#define GLOBALS_MATH_H

#include "common.h"

extern u32 gRandomValue;    /* LCG seed A (Random_Next) */
extern u32 gRandomValueB;   /* LCG seed B (Random_NextB) */
extern f32 gQuaternionSin;  /* the inlined axis-angle quaternion builder's sin of the half angle */

extern f32 gAcosTable[1024];    /* Math_AcosLookup, interpolated */

#endif
