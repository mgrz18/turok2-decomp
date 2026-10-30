#include "common.h"

/* Picks one of the four spawn points at random, at least 460.8 away from the module object's target (when
 * it has one), and stores it in out. */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ u8 padC[0x14 - 0xC];
} SpawnPoint;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3f pos;
} Actor;

typedef struct {
    /* 0x000 */ void *obj;
} State;

extern State D_8012FB70;
extern SpawnPoint D_00431D20[4];
extern f32 func_0040BD60(void *obj, Actor **found);
extern f32 func_002119FC(f32 min, f32 max);
extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b); /* out = a - b */
extern f32 func_00298470(f32 x); /* sqrt */

void func_0040C19C(Vec3f *out) {
    Actor *found;
    s32 ok;
    s32 i;

    func_0040BD60(D_8012FB70.obj, &found);
    do {
        ok = 1;
        i = func_002119FC(0.0f, 0.99999f) * 4.0f;
        if (found != NULL) {
            func_0020EF2C(out, &found->pos, &D_00431D20[i].pos);
            if (func_00298470(out->x * out->x + out->y * out->y + out->z * out->z) < 460.8f) {
                ok = 0;
            }
        }
    } while (!ok);
    *out = D_00431D20[i].pos;
}
