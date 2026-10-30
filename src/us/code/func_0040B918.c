#include "common.h"

/* AI state: turn toward the point (0, 0, 512) and go to anim state 0x1C once within 51.2 of it. */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3f pos;
} Obj;

extern f32 func_00246310(Obj *obj, Vec3f target);
extern void func_00245BAC(Obj *obj, void *ai, f32 angle, f32 turn);
extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b); /* out = a - b */
extern f32 func_00298470(f32 x); /* sqrt */
extern void func_00243414(Obj *obj, void *ai, s32 state);

void func_0040B918(Obj *obj, void *ai) {
    Vec3f d;

    d.x = 0.0f;
    d.y = 0.0f;
    d.z = 512.0f;
    func_00245BAC(obj, ai, 0.261799425f, func_00246310(obj, d));
    func_0020EF2C(&d, &d, &obj->pos);
    if (func_00298470(d.x * d.x + d.y * d.y + d.z * d.z) < 51.199997f) {
        func_00243414(obj, ai, 0x1C);
    }
}
