#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;
typedef struct { f32 x, y, z, w; } Quatf;
typedef f32 Mtx4f[4][4];

extern s32 func_00275F7C(void);
extern void *func_00275C6C(s32 bank, s32 sound, Vec3f *pos, Vec3f *pos2, s32 loops);  /* starts a sound */
extern void func_00275D34(s32 bank, void *handle);                                  /* stops a sound */
extern void func_00232B40(void *obj);

extern f32 func_0026D210(s32 half);                         /* half float -> f32 */
extern f32 func_00298470(f32);                              /* sqrtf */
extern void func_0020EF60(Vec3f *out, Vec3f *in, f32 s);    /* out = in * s */
extern void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);  /* out = a + b */
extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);  /* out = a - b */
extern void func_0020F040(Vec3f *v);                        /* normalize */
extern void func_0020E810(Quatf *out, Vec3f *dir);          /* rotation facing dir */
extern void func_00211074(Quatf *q);                        /* normalize */
extern void func_0020DCC8(Quatf *out, f32 t, Quatf *a, Quatf *b); /* slerp */
extern void func_00211114(Quatf *q, Mtx4f m);               /* to matrix */
extern void func_0020F85C(Mtx4f m, Vec3f *in, Vec3f *out);  /* transform */
extern void func_00229618(void *obj, f32 scale);

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
    /* 0x000 */ u8 pad0[0x214];
    /* 0x214 */ Vec3f pos;
} Target;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 state;
    /* 0x0C */ s32 sound;
} SoundHandle;

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[0x18 - 0x10];
    /* 0x018 */ Vec3f vel;
    /* 0x024 */ u8 pad24[0x40 - 0x24];
    /* 0x040 */ s32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ ParentDef *parent;
    /* 0x0FC */ u8 padFC[0x114 - 0xFC];
    /* 0x114 */ Target *target;
    /* 0x118 */ u8 pad118[0x11C - 0x118];
    /* 0x11C */ f32 speed;
    /* 0x120 */ u8 pad120[0x150 - 0x120];
    /* 0x150 */ Vec3f dir;
    /* 0x15C */ u8 pad15C[0x1B8 - 0x15C];
    /* 0x1B8 */ SoundHandle *loop;
} Missile;

/* Same as func_00231EF8: accelerate along dir, clamp to the max speed. */
static __inline__ void Missile_Accelerate(Missile *obj) {
    Vec3f delta;
    Vec3f dir;
    Vec3f *vel;
    f32 accel;
    f32 speed;
    f32 maxSpeed;

    accel = func_0026D210(obj->parent->move->accel);
    if (accel != 0.0f) {
        dir = obj->dir;
        func_0020EF60(&delta, &dir, accel);
        vel = &obj->vel;
        func_0020EEF8(vel, vel, &delta);
        speed = func_00298470(obj->vel.x * obj->vel.x + obj->vel.y * obj->vel.y + obj->vel.z * obj->vel.z);
        maxSpeed = func_0026D210(obj->parent->move->maxSpeed);
        if (accel > 0.0f) {
            if (maxSpeed < speed) {
                func_0020EF60(vel, &dir, maxSpeed);
            }
        } else {
            if (speed < maxSpeed) {
                func_0020EF60(vel, &dir, maxSpeed);
            }
        }
    }
}

/* A homing missile: keeps its flight sound 0x1B2 playing, turns toward its
 * target (aiming above it when far), then accelerates. */
void func_002298C4(Missile *obj) {
    Vec3f targetPos;
    Vec3f toTarget;
    Vec3f forward;
    Quatf cur;
    Quatf want;
    Quatf rot;
    Mtx4f mtx;
    Target *target;
    f32 speed;
    f32 dist;
    f32 t;
    f32 far;
    SoundHandle *loop;
    s32 sound;

    if (obj->flags & 0x10000) {
        if (obj->loop != NULL) {
            func_00275D34(func_00275F7C(), obj->loop);
            obj->loop = NULL;
        }
        return;
    }
    loop = obj->loop;
    sound = 0x1B2;
    if (loop != NULL) {
        if (loop->sound == sound && loop->state != -1) {
            goto playing;
        }
        func_00232B40(obj);
    }
    obj->loop = func_00275C6C(func_00275F7C(), 0x1B2, &obj->pos, &obj->pos, -1);
playing:

    target = obj->target;
    if (target != NULL && obj->speed > 7.0f) {
        func_0020E810(&cur, &obj->dir);
        speed = func_00298470(obj->vel.x * obj->vel.x + obj->vel.y * obj->vel.y + obj->vel.z * obj->vel.z);
        targetPos = target->pos;
        func_0020EF2C(&toTarget, &targetPos, &obj->pos);
        dist = func_00298470(toTarget.x * toTarget.x + toTarget.y * toTarget.y + toTarget.z * toTarget.z);
        if (dist < 102.399994f) {
            func_0020E810(&rot, &toTarget);
            func_00211114(&rot, mtx);
            forward.x = 0.0f;
            forward.y = 0.0f;
            forward.z = 1.0f;
            func_0020F85C(mtx, &forward, &obj->dir);
            func_0020EF60(&obj->vel, &obj->dir, speed);
            return;
        }
        if (dist > 153.59999f) {
            targetPos.y += 102.399994f;
            func_0020EF2C(&toTarget, &targetPos, &obj->pos);
            dist = func_00298470(toTarget.x * toTarget.x + toTarget.y * toTarget.y + toTarget.z * toTarget.z);
        }
        func_0020F040(&toTarget);
        func_0020E810(&want, &toTarget);
        if (dist < 1024.0f) {
            far = dist - 204.79999f;
            if (far < 0.0f) {
                t = 0.0f;
            } else {
                t = far * 0.0012207031f;
            }
            t = 1.0f - t * 0.85999995f;
        } else {
            t = 0.04f;
        }
        func_00211074(&want);
        func_00211074(&cur);
        func_0020DCC8(&rot, t, &cur, &want);
        func_00211114(&rot, mtx);
        forward.x = 0.0f;
        forward.y = 0.0f;
        forward.z = 1.0f;
        func_0020F85C(mtx, &forward, &obj->dir);
        func_0020EF60(&obj->vel, &obj->dir, speed);
    }
    Missile_Accelerate(obj);
    func_00229618(obj, 1.0f);
}
