#ifndef STRUCTS_LEVEL_ACTOR_H
#define STRUCTS_LEVEL_ACTOR_H

/* The level's objects that are not a CGameObjectInstance: the static
 * scenery objects (CStaticObject, 0x90 bytes), the level actors
 * (CLevelActor, 0x198 bytes: placed with the level, or spawned at run time
 * from the world's pool, CLevelActorPool, as the pickups are) and the pool's
 * slots (CPooledLevelActor, the actor plus its list links).
 *
 * All of them start like an instance (CInstanceHeader: type, id, position,
 * region, intelligence), which is why the collision and drawing code can
 * take any of them. The level geometry's display list (CScene__BuildGeometryDisplayList,
 * CScene__DrawGeometry) draws both kinds; the level loader (CScene__DecompressGridSection) sets
 * the actors' killed bit and their lights; the pool functions
 * (CLevelActorPool__Spawn, CLevelActorPool__Update, CPooledLevelActor__Release, CLevelActorPool__Alloc,
 * SceneActorPool_Construct_2) spawn and release the pooled ones. Fields named unk are not
 * known yet. */

#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"
#include "structs/collision.h"
#include "structs/light.h"

/* CStaticObject.m_wFlags / CLevelActor.m_wFlags */
#define LEVEL_ACTOR_FLAG_DEAD        0x0001 /* killed (the level's killed bit is set) */
#define LEVEL_ACTOR_FLAG_SHOWN       0x0002
#define LEVEL_ACTOR_FLAG_GROWING     0x0010 /* scaling in to 0.35 (CLevelActor__UpdateGrowIn) */

/* A static scenery object of the level, 0x90 bytes. */
typedef struct CStaticObject {
    /* 0x00 */ CInstanceHeader header;  /* m_pCurrentRegion: its segment (region), m_pIntelligence: its descriptor */
    /* 0x18 */ s32 m_ObjectAddr;        /* the object's ROM address (CISet__GetROMAddress): CStaticObject__LoadGeometry */
    /* 0x1C */ s32 m_GeometryAddr;      /* its geometry's ROM address, found once */
    /* 0x20 */ s32 m_GeometrySize;
    /* 0x24 */ void **m_pOwner;         /* the grid section's cache handle (CScene__DrawGeometry) */
    /* 0x28 */ f32 m_Matrix[16];        /* an Mtx (CMtxF__ToMtxAffine), or a CMtxF with flag 0x40 */
    /* 0x68 */ CBox m_Bounds;
    /* 0x80 */ s32 m_LitKey;            /* the cache key of its lit vertices (| gLitVerticesKey) */
    /* 0x84 */ s32 m_ViewMask;          /* the views it shows in */
    /* 0x88 */ u16 m_wFlags;            /* 4: translucent, 8: listed, 0x40: no geometry */
    /* 0x8A */ u8 m_LitFrame;           /* the frame its colours were last pulsed */
    /* 0x8B */ u8 pad8B[0x90 - 0x8B];
} CStaticObject; /* size 0x90; host 0xA0 */
#ifdef TARGET_PORT
/* The files that read a static object through their own view (PlacedProp, SpawnSource, Drawable)
 * declare a host twin of it; these sizes keep both in step. */
#define T2_STATIC_OBJECT_CHECK(T, matrix, flags) \
    _Static_assert(sizeof(T) == sizeof(CStaticObject), #T " size"); \
    _Static_assert(__builtin_offsetof(T, matrix) == __builtin_offsetof(CStaticObject, m_Matrix), #T " matrix"); \
    _Static_assert(__builtin_offsetof(T, flags) == __builtin_offsetof(CStaticObject, m_wFlags), #T " flags")
#endif

/* A level actor, 0x198 bytes: set up from its placement record by
 * CLevelActor__Construct, drawn by CLevelActor__Draw. The first 0x34 bytes are a
 * CObjectHead. */
typedef struct CLevelActor {
    /* 0x000 */ CInstanceHeader header;     /* m_Type 3; m_nObjType: the index in the level's id table */
    /* 0x018 */ CVector3 m_vVelocity;
    /* 0x024 */ s32 unk24[4];               /* CObjectHead's water displacement and m_pInstanceBelow, cleared on a spawn */
    /* 0x034 */ s32 m_Resource;             /* CScene__GetObject (the model) */
    /* 0x038 */ f32 m_Matrices[4][16];      /* its matrix, one per frame slot (gEvenOdd), rebuilt once per frame */
    /* 0x138 */ s32 unk138;
    /* 0x13C */ f32 m_Scale;
    /* 0x140 */ s32 unk140;
    /* 0x144 */ f32 m_RotY;
    /* 0x148 */ s32 unk148;
    /* 0x14C */ CBox m_Bounds;
    /* 0x164 */ f32 m_ModelScale;           /* 0.35 when placed */
    /* 0x168 */ f32 m_Height;
    /* 0x16C */ u16 m_wFlags;               /* LEVEL_ACTOR_FLAG_*: 1 dead, 2 shown, 0x20 translucent */
    /* 0x16E */ u16 m_nKilledIndex;         /* its bit in the level's killed array */
    /* 0x170 */ f32 m_Timer;                /* the grow-in time (flag 0x10, CLevelActor__UpdateGrowIn), the morpher's animation clock (CLevelActor__Update) */
    /* 0x174 */ f32 m_AnimSpeed;            /* type 12 (morpher): the frame rate times its m_AnimSpeed (CLevelActor__UpdateAnimSpeed) */
    /* 0x178 */ CLights1 m_Lights;           /* the lighting at its position (CScene__GetLightsAt, CScene__ComputeLights; gSceneLights when placed) */
    /* 0x190 */ s32 m_ViewMask;             /* the views it shows in: the placement record's, then its region's m_VisBits (CLevelActor__Update) */
    /* 0x194 */ u8 m_DrawnFrame;            /* the frame (D_800B6D1B) its matrix was last built for */
    /* 0x195 */ u8 m_UpdatedFrame;          /* the frame its once-per-frame update (CLevelActor__Update) last ran */
    /* 0x196 */ u8 pad196[0x198 - 0x196];
} CLevelActor; /* size 0x198 */

/* A slot of the world's level actor pool: the actor, its lifetime and its
 * links in the pool's lists. */
typedef struct CPooledLevelActor {
    /* 0x000 */ CLevelActor m_Actor;
    /* 0x198 */ f32 m_Lifetime;
    /* 0x19C */ f32 m_Timer;
    /* 0x1A0 */ s32 m_PoolFlags;            /* 1: in use */
    /* 0x1A4 */ s32 *m_pOwnerCount;         /* counted down when it goes */
    /* 0x1A8 */ struct CPooledLevelActor *m_pPrev;
    /* 0x1AC */ struct CPooledLevelActor *m_pNext;
} CPooledLevelActor; /* size 0x1B0 */

/* The world's level actor pool (CScene.m_ActorPool, 0x10518): 32 slots, the
 * free list and the live list (newest at the head, oldest at the tail,
 * released first when no slot is free). */
typedef struct CLevelActorPool {
    /* 0x0000 */ CPooledLevelActor m_Actors[32];
    /* 0x3600 */ CPooledLevelActor *m_pFree;
    /* 0x3604 */ CPooledLevelActor *m_pHead;
    /* 0x3608 */ CPooledLevelActor *m_pTail;
} CLevelActorPool; /* size 0x360C */

#endif /* STRUCTS_LEVEL_ACTOR_H */
