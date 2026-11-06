#include "structs/vector.h"
#include "globals/scene.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/80206258.CPP, func_802063EC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
struct Triple;
struct Triple;
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
struct Triple;
typedef struct Triple Triple;
typedef struct Shared_ParticleLighting Shared_ParticleLighting;
struct Shared_ParticleLighting {
    u8 ambient[3], reserved3;
    u8 ambientCopy[3], reserved7;
    u8 diffuse[3], reservedB;
    u8 diffuseCopy[3], reservedF;
    s8 direction[3];
    u8 reserved13[5];
} __attribute__((aligned(4)));
struct Triple;
struct Triple;
struct Triple;
/* Turok 2 layouts (Rage Wars offsets in comments). */
typedef struct T2Actor {
    u8 pad0[0x18];
    CVector3 position;         /* 0x18 (0x1C) */
    u8 pad24[0x50 - 0x24];
    f32 yaw;                   /* 0x50 (0x6C) */
} T2Actor;

typedef struct T2MotionState {
    u8 pad0[0x10C];
    s32 unk10C;                /* 0x10C (0x124) */
    s32 counter;               /* 0x110 (0x128) */
} T2MotionState;

extern s32 CInstancePool__Spawn(void *arg0, s32 arg1, s32 arg2, s32 arg3,
                         s32 arg4, Triple arg5, f32 arg6, CVector3 arg7,
                         Triple arg8, void *arg9);
void Generator_SpawnObject(T2Actor *arg0, T2MotionState *arg1, Triple arg2, Triple arg3,
                   s32 arg4, s32 arg5, s32 arg6) {
    if (CInstancePool__Spawn(&gScene.m_InstancePool, arg5, 0, arg6,
                      arg4, arg2,
                      arg0->yaw,
                      arg0->position, arg3,
                      &arg1->unk10C) != 0) {
        arg1->counter -= 1;
    }
}
