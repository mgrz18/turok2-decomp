#include "common.h"

/* Spawns effect 3 from obj at other's position, with params (0xA00, value << 8). */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ s16 upper;
    /* 0x02 */ s16 lower;
    /* 0x04 */ u8 extra[4];
} __attribute__((aligned(4))) ActionParams;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3f pos;
} Obj;

extern void func_00236EA0(void *source, void *p, s32 effect, s32 arg3, Vec3f pos, ActionParams params);

void func_0040C518(Obj *obj, Obj *other, s32 value) {
    ActionParams params;

    params.upper = 0xA00;
    params.lower = value << 8;
    params.extra[0] = params.extra[1] = params.extra[2] = params.extra[3] = 0;
    func_00236EA0(obj, obj, 3, -1, other->pos, params);
}
