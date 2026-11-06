#ifndef GLOBALS_COLLISION_H
#define GLOBALS_COLLISION_H

/* The collision results and the collision probes (structs/collision.h). */

#include "structs/collision.h"

extern CCollisionResults gCollisionResults;    /* Turok 3's collision_results */
extern CCollisionResults *gCollisionResultsP;  /* Turok 3's p_collision_results */
extern CCollisionInfo gCollisionInfoSight;         /* eye-line probes */
extern CCollisionInfo gCollisionInfoTestMove;      /* GameObject_TestMove, an actor that can leave water */
extern CCollisionInfo gCollisionInfoLineOfSight;   /* line-of-sight probes */
extern CCollisionInfo gCollisionInfoTestMoveWater; /* GameObject_TestMove, an actor that cannot */

#endif
