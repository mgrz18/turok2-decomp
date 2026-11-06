#ifndef GLOBALS_COLLISION_H
#define GLOBALS_COLLISION_H

/* The collision results and the collision probes (structs/collision.h). */

#include "structs/collision.h"
#include "common.h"
#include "structs/vector.h"

extern CCollisionResults gCollisionResults;    /* Turok 3's collision_results */
extern CCollisionResults *gCollisionResultsP;  /* Turok 3's p_collision_results */
extern CCollisionInfo gCollisionInfoSight;         /* eye-line probes */
extern CCollisionInfo gCollisionInfoTestMove;      /* GameObject_TestMove, an actor that can leave water */
extern CCollisionInfo gCollisionInfoLineOfSight;   /* line-of-sight probes */
extern CCollisionInfo gCollisionInfoTestMoveWater; /* GameObject_TestMove, an actor that cannot */

/* Field symbols of gCollisionResults, kept where the code reaches the
 * field by its own %hi/%lo (CParticle__HitWall/HitWater/HitFloor/HitCeiling). */
extern CGameRegion *gCollisionResults_m_pWallRegion;
extern CVector3 gCollisionResults_m_vWallPos;
extern s32 gCollisionResults_m_nWallEdge;
extern CGameRegion *gCollisionResults_m_pFloorRegion;
extern CVector3 gCollisionResults_m_vFloorPos;
extern CGameRegion *gCollisionResults_m_pWaterRegion;
extern CVector3 gCollisionResults_m_vWaterPos;
extern s32 gCollisionResults_m_nWaterKind;
extern CVector3 gCollisionResults_m_vVelocity;

extern void (*sCollisionTests[8])(Sweep *sweep);    /* NULL-ended; Collision_MoveInstance */
/* Quad_SetBoxTop, Quad_SetBoxSide*: NULL-ended (Platform_CollideBox). */
extern void (*sBoxSideFaceBuilders[8])(CollisionQuad *quad, CVector3 *points);

/* The engine objects' collision setups (Turok 3's colinfo.c name), at the
 * head of the player code's file in the first VM module, and its module-2
 * copy (written by hand in asm/data_0046642C.s). The last record ends the
 * table (NULL m_pInfo). */

extern CCollisionSetup gCollisionSetupTable[34];
extern CCollisionSetup gCollisionSetupTable_2[34];
extern MapSweep *sMapSweep;

/* The throwables' CCollisionInfo (0x800F1F90): fields with symbols of their own */
extern s8 gThrowableCollisionInfo_m_InstanceBehavior;
extern f32 gThrowableCollisionInfo_m_GravityAcceleration;

#endif
