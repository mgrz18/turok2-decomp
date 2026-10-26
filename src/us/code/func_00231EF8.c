#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;

extern f32 func_0026D210(s32 half);                        /* half float -> f32 */
extern void func_0020EF60(Vec3f *out, Vec3f *in, f32 s);   /* out = in * s */
extern void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b); /* out = a + b */
extern f32 func_00298470(f32);                             /* sqrtf */

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 accel;      /* half floats */
    /* 0x12 */ u16 maxSpeed;
} MoveParams;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ MoveParams *move;
} ParentDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ Vec3f vel;
    /* 0x024 */ u8 pad24[0xF8 - 0x24];
    /* 0x0F8 */ ParentDef *parent;
    /* 0x0FC */ u8 padFC[0x150 - 0xFC];
    /* 0x150 */ Vec3f dir;
} Mover;

/* Accelerates obj->vel along obj->dir, clamping the speed to the parent limit. */
void func_00231EF8(Mover *obj) {
    Vec3f delta;
    Vec3f dir;
    Vec3f *pdir;
    Vec3f *vel;
    f32 accel;
    f32 speed;
    f32 maxSpeed;

    accel = func_0026D210(obj->parent->move->accel);
    if (accel != 0.0f) {
        pdir = &dir;
        *pdir = obj->dir;
        func_0020EF60(&delta, pdir, accel);
        vel = &obj->vel;
        func_0020EEF8(vel, vel, &delta);
        speed = func_00298470(obj->vel.x * obj->vel.x + obj->vel.y * obj->vel.y + obj->vel.z * obj->vel.z);
        maxSpeed = func_0026D210(obj->parent->move->maxSpeed);
        if (accel > 0.0f) {
            if (maxSpeed < speed) {
                func_0020EF60(vel, pdir, maxSpeed);
            }
        } else {
            if (speed < maxSpeed) {
                func_0020EF60(vel, pdir, maxSpeed);
            }
        }
    }
}
