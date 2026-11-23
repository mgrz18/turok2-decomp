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
/* The other infos gCollisionSetupTable fills (round 24 m2; versions/names/data_g5.us.txt) */
extern CCollisionInfo gCollisionInfoTestMoveClimb; /* GameObject_TestMove, an actor that can climb up */
extern CCollisionInfo gCollisionInfoParticle;      /* each new particle effect's template */
extern CCollisionInfo gCollisionInfoTestMoveLand;  /* GameObject_TestMove, an actor that cannot enter water */
extern CCollisionInfo gCollisionInfoEnemy;         /* the enemy mode tables' m_pData */
extern CCollisionInfo gCollisionInfoEnemyB;        /* the same values; the Primagen toggles its flags */
extern CCollisionInfo gCollisionInfoNoInstances;   /* never tested against instances */
extern CCollisionInfo gCollisionInfoTrace;         /* GameObject_TraceMove, GameObject_DropPickupAt */
extern CCollisionInfo gCollisionInfoPlatformRider; /* AI_RideMovingPlatform */
extern CCollisionInfo gCollisionInfoNoGravity;     /* player state 14, backwards */
extern CCollisionInfo gCollisionInfoModeDefault;   /* a mode without m_pData */
extern CCollisionInfo gCollisionInfoPooledActor;   /* CPooledLevelActor__Update */
extern CCollisionInfo gCollisionInfoGravity;       /* player state 14, forwards; MotherBoss_Init */
extern CCollisionInfo gCollisionInfoFlyingEnemy;   /* the Primagen's modes (no gravity) */

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
