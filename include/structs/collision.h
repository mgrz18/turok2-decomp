#ifndef STRUCTS_COLLISION_H
#define STRUCTS_COLLISION_H

/* Collision: an object's collision response (CCollisionInfo), the move being
 * tested (Sweep, func_00239F00 builds one on its stack and runs the tests of
 * D_800AD190 on it), and what the last test hit (CCollisionResults,
 * D_800F1CE0; D_800F1ADC points at the one being filled).
 *
 * A test that takes a hit records where to write it back (SweepHit's *Out
 * pointers into the results); func_0023C120 writes the object's position,
 * its region, the kind and the data there when the move ends on that hit.
 *
 * CCollisionInfo is Rage Wars' CCollisionInfo2_t (rw_abu
 * include/COMMON/CCOLLISIONINFO2_T.H, filled from a CCollisionSetup_t table
 * by func_002418C0); "CCollisionInfo__Reset" and gCollisionResults /
 * gCollisionResultsP are Turok 3's names (references/turok3 colinfo.h).
 * The floor triangles a sweep walks are CGameRegion (structs/region.h).
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "structs/game_object.h"
#include "structs/region.h"

/* The behaviours (m_InstanceBehavior, m_WallBehavior, m_GroundBehavior, and
 * the kind func_0023AB18 takes) */
#define COLLIDE_NONE    0
#define COLLIDE_STOP    1
#define COLLIDE_SLIDE   2
#define COLLIDE_BOUNCE  3
#define COLLIDE_THROUGH 7

/* An object's collision response. */
typedef struct CCollisionInfo {
    /* 0x00 */ u32 m_dwFlags;
    /* 0x04 */ s8 m_InstanceBehavior;    /* against instances */
    /* 0x05 */ s8 m_WallBehavior;        /* against walls (and models) */
    /* 0x06 */ s8 m_GroundBehavior;      /* against the floor and ceilings */
    /* 0x07 */ s8 m_unusedpad;
    /* 0x08 */ f32 m_GravityAcceleration;
    /* 0x0C */ f32 m_BounceReturnEnergy;
    /* 0x10 */ f32 m_GroundFriction;
    /* 0x14 */ f32 m_AirFriction;
    /* 0x18 */ f32 m_WaterFriction;
} CCollisionInfo; /* size 0x1C */

/* What a hit on an instance's node copies into the results (0x70 bytes). */
typedef struct HitFrame {
    /* 0x00 */ s32 unk_00[0x19];
    /* 0x64 */ u8 unk_64;
    /* 0x65 */ u8 m_Surface;
    /* 0x66 */ u8 m_Team;
    /* 0x67 */ u8 unk_67;
    /* 0x68 */ s32 unk_68[2];
} HitFrame; /* size 0x70 */

/* What the last collision test hit (D_800F1CE0, 0xF0), one record per kind
 * of thing: each has the region the object was in, where it was, and a kind
 * or the thing hit. func_00241894 clears it (the instance's node to -1). */
typedef struct CCollisionResults {
    /* 0x00 */ CGameObjectInstance *m_pInstance;   /* the instance hit (or the model) */
    /* 0x04 */ CGameRegion *m_pInstanceRegion;
    /* 0x08 */ CVector3 m_vInstancePos;            /* the collision point */
    /* 0x14 */ s32 m_nInstanceNode;                /* the node (bone) hit, -1 none */
    /* 0x18 */ HitFrame m_Frame;                    /* what the hit node gave (func_0023F880) */
    /* 0x88 */ CGameObjectInstance *m_pShape;      /* an instance hit by its collision shape (func_00242604) */
    /* 0x8C */ CGameRegion *m_pShapeRegion;
    /* 0x90 */ CVector3 m_vShapePos;
    /* 0x9C */ CGameRegion *m_pWallRegion;         /* non-NULL: a wall was hit */
    /* 0xA0 */ CVector3 m_vWallPos;
    /* 0xAC */ s32 m_nWallEdge;                    /* the region edge (0..2) */
    /* 0xB0 */ s32 m_nFloorKind;                   /* 1 floor, 2 ceiling */
    /* 0xB4 */ CGameRegion *m_pFloorRegion;
    /* 0xB8 */ CVector3 m_vFloorPos;
    /* 0xC4 */ CGameRegion *m_pWaterRegion;        /* or a model's special hit */
    /* 0xC8 */ CVector3 m_vWaterPos;
    /* 0xD4 */ s32 m_nWaterKind;
    /* 0xD8 */ CVector3 m_vVelocity;               /* the mover's velocity */
    /* 0xE4 */ CVector3 m_vNormal;                 /* the normal of the plane hit */
} CCollisionResults; /* size 0xF0 */

/* The nearest hit a sweep found so far, and where to record it. */
typedef struct SweepHit {
    /* 0x00 */ f32 fraction;                   /* of the move done before the hit */
    /* 0x04 */ s32 type;                       /* 0 none, 1 slide, 2 stop, 3 go on to slideTo */
    /* 0x08 */ CVector3 vel;                   /* velocity after the hit */
    /* 0x14 */ CVector3 slideTo;
    /* 0x20 */ CVector3 pos;                   /* where the hit is */
    /* 0x2C */ CVector3 *posOut;               /* where func_0023C120 writes the object's position */
    /* 0x30 */ CGameRegion **regionOut;        /* ... its region */
    /* 0x34 */ s32 *kindOut;                   /* ... kind */
    /* 0x38 */ s32 kind;
    /* 0x3C */ void **dataOut;                 /* ... data */
    /* 0x40 */ void *data;
} SweepHit; /* size 0x44 */

/* A move from obj's position to end, as func_00239F00 tests it. */
typedef struct Sweep {
    union {
    /* 0x00 */ CGameObjectInstance *obj;
    /* 0x00 */ CInstanceHeader *inst;
    };
    /* 0x04 */ CCollisionInfo *def;
    /* 0x08 */ SweepHit hit;
    /* 0x4C */ CVector3 end;
    /* 0x58 */ CVector3 delta;                 /* end - the start */
    /* 0x64 */ CVector3 lastContact;           /* the last hit's normal (or cylinder centre) */
    /* 0x70 */ CVector3 contact;               /* this hit's */
    /* 0x7C */ f32 lastRadius;                 /* the last cylinder's radius */
    /* 0x80 */ f32 radius;
    /* 0x84 */ s32 lastKind;                   /* the last hit's kind (1: sliding) */
    /* 0x88 */ s32 kind;
    /* 0x8C */ s32 more;                       /* another pass follows */
    /* 0x90 */ s32 physics;                    /* velocity and gravity are applied */
    /* 0x94 */ s32 onGround;
    /* 0x98 */ s32 checkRegionInfo;
    /* 0x9C */ s32 stop;                       /* stops the tests */
    /* 0xA0 */ s32 unkA0;
    /* 0xA4 */ u8 padA4[4];
} Sweep; /* size 0xA8 */

/* A sweep against a model, in the sweep's own space (func_0023D050 builds
 * it, func_0023DBBC and func_0023D7CC walk the model's polygons). */
typedef struct ModelSweep {
    /* 0x00 */ Sweep *sweep;
    /* 0x04 */ f32 view[4][4];                 /* world to sweep space */
    /* 0x44 */ f32 scale;                      /* minus the sweep's length */
    /* 0x48 */ f32 hitDist;                    /* scale * the nearest hit's fraction */
    /* 0x4C */ void *model;                    /* the scene model */
    /* 0x50 */ s32 kind;                       /* func_00245724(model) */
    /* 0x54 */ f32 mtx[4][4];                  /* the model's matrix */
    /* 0x94 */ f32 inverse[4][4];
    /* 0xD4 */ void *meshes;
    /* 0xD8 */ s32 special;                    /* kind 9: every polygon counts */
} ModelSweep; /* size 0xDC */

/* The volume a sweep moves, against instances' cylinders (func_0024032C
 * builds it): the mover's radius and height, its top and bottom at the start
 * and the end, and the ground rectangle the move covers. */
typedef struct SweepBox {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 bottom;                     /* below the position */
    /* 0x08 */ f32 top;                        /* above it */
    /* 0x0C */ f32 startTop;
    /* 0x10 */ f32 startBottom;
    /* 0x14 */ f32 endTop;
    /* 0x18 */ f32 endBottom;
    /* 0x1C */ f32 minX;
    /* 0x20 */ f32 minZ;
    /* 0x24 */ f32 maxX;
    /* 0x28 */ f32 maxZ;
    /* 0x2C */ s32 flag;                       /* the move goes up or down: test the discs */
} SweepBox; /* size 0x30 */

/* A sweep's circle against the map's wall edges (func_00240E90 builds it on
 * its stack and D_800F1AB0 points at it while func_00240800 walks the
 * regions). */
typedef struct MapSweep {
    /* 0x00 */ Sweep *sweep;
    /* 0x04 */ CVector3 *a;                    /* the edge's corners */
    /* 0x08 */ CVector3 *b;
    /* 0x0C */ CVector3 side;                  /* the sweep's direction turned a right angle */
    /* 0x18 */ CVector3 edge;                  /* b - a */
    /* 0x24 */ CVector3 toEnd;
    /* 0x30 */ CVector3 wallPoint;             /* a pushed out by the radius */
    /* 0x3C */ CVector3 normal;                /* the edge's normal */
    /* 0x48 */ CVector3 toStart;
    /* 0x54 */ f32 radius;
    /* 0x58 */ f32 len;
    /* 0x5C */ f32 startSide;
    /* 0x60 */ f32 across;
    /* 0x64 */ f32 endSide;
    /* 0x68 */ f32 t;
    /* 0x6C */ CGameRegion *next;
    /* 0x70 */ s32 hit;
    /* 0x74 */ f32 minX;                       /* the ground rectangle the move covers */
    /* 0x78 */ f32 minZ;
    /* 0x7C */ f32 maxX;
    /* 0x80 */ f32 maxZ;
    /* 0x84 */ u8 pad84[4];
} MapSweep; /* size 0x88 */

#endif
