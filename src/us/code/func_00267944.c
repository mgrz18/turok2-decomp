/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0024D860.c, func_8024D870_de, with Turok 2's layouts. */
#include "common.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Quat {
    f32 x, y, z, w;
} Quat;

typedef struct Actor {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[3];
    /* 0x004 */ f32 x;
    /* 0x008 */ f32 y;
    /* 0x00C */ f32 z;
    /* 0x010 */ void *region;
    /* 0x014 */ u8 pad14[0x50 - 0x14];
    /* 0x050 */ f32 yaw;
    /* 0x054 */ u8 pad54[0xD4 - 0x54];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x140 */ s32 motionState;
} Actor;

extern f32 D_800DD78C;

s32 func_0026AFC0(void *region);
void func_0026BD20(Vec3 *normal, void *region);
void func_0020EFDC(Vec3 *out, Vec3 *a, Vec3 *b);     /* cross product */
void func_0020F040(Vec3 *v);                         /* normalize */
f32 func_0021153C(f32 x);                            /* acos */
f32 func_002982F0(f32 x);                            /* sin */
f32 func_002974C0(f32 x);                            /* cos */
f32 func_0026A268(void *region, f32 x, f32 z, f32 toX, f32 toZ);  /* ground slope */
void func_0020DCC8(Quat *out, f32 t, Quat *a, Quat *b);           /* slerp */

static __inline__ s32 isHeld(Actor *obj) {
    return (obj->kind != 1) ? 0 : (obj->flags & 1);
}

/* Tilt of an actor toward the floor normal under it. */
static __inline__ Quat floorTilt(Actor *obj) {
    Vec3 normal;
    Vec3 up;
    Vec3 axis;
    Quat q;
    f32 angle;

    func_0026BD20(&normal, obj->region);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    func_0020EFDC(&axis, &up, &normal);
    func_0020F040(&axis);
    angle = func_0021153C(up.x * normal.x + up.y * normal.y + up.z * normal.z) * 0.5f;
    D_800DD78C = func_002982F0(angle);
    q.x = axis.x * D_800DD78C;
    q.y = axis.y * D_800DD78C;
    q.z = axis.z * D_800DD78C;
    q.w = func_002974C0(angle);
    return q;
}

/* An actor's body lean on the ground: players blend the floor tilt with a lean along their heading. */
Quat func_00267944(Actor *obj) {
    Vec3 normal;
    Vec3 up;
    Vec3 axis;
    Quat tilt;
    Quat lean;
    Quat result;
    f32 slope;
    f32 sine;
    f32 cosine;
    f32 angle;
    f32 half;
    Actor *self;

    if (obj->region != NULL && isHeld(obj) == 0) {
        if (obj->kind == 1) {
            self = obj;
            if (obj->motionState & 0xC0000) {
                goto identity;
            }
            if (obj->flags & 0x300000) {
                if (func_0026AFC0(obj->region) != 0) {
                    goto identity;
                }
                func_0026BD20(&normal, obj->region);
                up.x = 0.0f;
                up.y = 1.0f;
                up.z = 0.0f;
                func_0020EFDC(&axis, &up, &normal);
                func_0020F040(&axis);
                slope = func_0021153C(up.x * normal.x + up.y * normal.y + up.z * normal.z);
                D_800DD78C = func_002982F0(MIN(slope, 0.43633235f) * 0.375f);
                tilt.x = axis.x * D_800DD78C;
                tilt.y = axis.y * D_800DD78C;
                tilt.z = axis.z * D_800DD78C;
                tilt.w = func_002974C0(MIN(slope, 0.43633235f) * 0.375f);
                sine = func_002982F0(self->yaw);
                cosine = func_002974C0(self->yaw);
                slope = func_0026A268(obj->region, obj->x, obj->z, obj->x - sine, obj->z - cosine);
                slope = MIN(slope, 25.0f) * 0.75f;
                half = 0.5f;
                angle = slope * half;
                D_800DD78C = func_002982F0(angle);
                lean.y = 0.0f;
                lean.x = cosine * D_800DD78C;
                lean.z = -sine * D_800DD78C;
                lean.w = func_002974C0(angle);
                func_0020DCC8(&result, half, &lean, &tilt);
                return result;
            }
        }
        return floorTilt(obj);
    }
identity:
    tilt.x = tilt.y = tilt.z = 0.0f;
    tilt.w = 1.0f;
    return tilt;
}
