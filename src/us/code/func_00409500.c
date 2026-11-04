/* First-module copy of func_00442500 (src/us/code/func_00442500.c). */
#include "common.h"
#include "structs/vector.h"

/* Spawns one actor near each of the four spawn points, scattered by up to +-15 units, skipping a point
 * closer than 460.8 to the target of the module object. */

typedef struct {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ u8 padC[0x14 - 0xC];
} SpawnPoint;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ CVector3 pos;
} Actor;

typedef struct {
    /* 0x000 */ void *obj;
} State;

extern State D_8012FB70;
extern SpawnPoint D_00431D20[4];
extern u8 D_800F7078[];
extern u8 D_80107590[];
extern f32 func_0040BD60(void *obj, Actor **found);
extern s32 func_0026D518(void *a);
extern f32 func_002119FC(f32 min, f32 max);
extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern f32 func_00298470(f32 x); /* sqrt */
extern s32 func_00220A1C(void *world, CVector3 *pos);
extern void *func_00227240(void *pool, s32 a1, s16 type, CVector3 vel, CVector3 pos, s32 region, f32 arg);
extern void func_00246690(void *actor, s32 id, CVector3 pos, s32 a5);

static __inline__ void SpawnAt(s32 arg0, s16 type, CVector3 pos, f32 speed) {
    CVector3 vel;
    s32 region;
    void *actor;

    region = func_00220A1C(D_800F7078, &pos);
    vel.x = 0.0f;
    vel.y = speed;
    vel.z = 0.0f;
    actor = func_00227240(D_80107590, 0, type, vel, pos, region, arg0);
    if (actor != NULL) {
        func_00246690(actor, 0x11D, pos, 0);
    }
}

void func_00409500(s32 arg0, void *arg1) {
    CVector3 v;
    Actor *found;
    s32 i;
    s32 type;
    s32 dx;
    s32 dz;

    func_0040BD60(D_8012FB70.obj, &found);
    for (i = 0; i < 4; i++) {
        type = (s16)func_0026D518(arg1);
        dx = func_002119FC(-15.0f, 15.0f);
        dz = func_002119FC(-15.0f, 15.0f);
        dx = dx * 10.24f;
        dz = dz * 10.24f;
        if (found != NULL) {
            func_0020EF2C(&v, &found->pos, &D_00431D20[i].pos);
            if (func_00298470(v.x * v.x + v.y * v.y + v.z * v.z) > 460.8f) {
                v = D_00431D20[i].pos;
                v.y = 30.72f;
                v.x += dx;
                v.z += dz;
                SpawnAt(arg0, type, v, 512.0f);
            }
        } else {
            v = D_00431D20[i].pos;
            v.y = 30.72f;
            v.x += dx;
            v.z += dz;
            SpawnAt(arg0, type, v, 512.0f);
        }
    }
}
