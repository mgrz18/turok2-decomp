#ifndef GLOBALS_COLLISION_H
#define GLOBALS_COLLISION_H

#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

extern void (*sCollisionTests[8])(Sweep *sweep);    /* NULL-ended; Collision_MoveInstance */
/* Quad_SetBoxTop, Quad_SetBoxSide*: NULL-ended (Platform_CollideBox). */
extern void (*sBoxSideFaceBuilders[8])(CollisionQuad *quad, CVector3 *points);

#endif
