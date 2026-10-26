#include "common.h"

/* Same family as func_002298C4 (homing missile): here a projectile that flies
 * back to a point beside its owner's player and is caught there. */

typedef struct { f32 x, y, z; } Vec3f;
typedef struct { f32 x, y, z, w; } Quatf;
typedef f32 Mtx4f[4][4];

extern s32 func_00275F7C(void);
extern void *func_00275C6C(s32 bank, s32 sound, Vec3f *pos, Vec3f *pos2, s32 loops);  /* starts a sound */
extern void func_00232B40(void *obj);

extern f32 func_0026D210(s32 half);                         /* half float -> f32 */
extern f32 func_00298470(f32);                              /* sqrtf */
extern f32 func_0021153C(f32);                              /* acosf */
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
    /* 0x000 */ u8 pad0[0x4D8];
    /* 0x4D8 */ f32 f4D8;
    /* 0x4DC */ u8 pad4DC[0x996 - 0x4DC];
    /* 0x996 */ s16 weapon;
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ Player *player;
} Owner;

extern void func_00253F90(Player *player, Vec3f *pos);      /* player-relative point to world */
extern f32 func_002671B4(Owner *owner);                     /* height */

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
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 state;
    /* 0x0C */ s32 sound;
} SoundHandle;

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[0x18 - 0x10];
    /* 0x018 */ Vec3f vel;
    /* 0x024 */ u8 pad24[0xF8 - 0x24];
    /* 0x0F8 */ ParentDef *parent;
    /* 0x0FC */ u8 padFC[0x10C - 0xFC];
    /* 0x10C */ Owner *owner;
    /* 0x110 */ u8 pad110[0x11C - 0x110];
    /* 0x11C */ f32 speed;
    /* 0x120 */ u8 pad120[0x128 - 0x120];
    /* 0x128 */ s16 caught;
    /* 0x12A */ u8 pad12A[0x150 - 0x12A];
    /* 0x150 */ Vec3f dir;
    /* 0x15C */ u8 pad15C[0x1B8 - 0x15C];
    /* 0x1B8 */ SoundHandle *loop;
} Missile;

extern f32 D_800ACE68; /* speed above which it flies home (15) */
extern f32 D_800ACE6C; /* catch radius (512) */

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

void func_00229D10(Missile *obj) {
    Vec3f aim;
    Vec3f toHome;
    Vec3f facing;
    Vec3f forward;
    Vec3f home;
    Vec3f toCatch;
    Quatf cur;
    Quatf want;
    Quatf rot;
    Mtx4f mtx;
    Player *player;
    SoundHandle *loop;
    f32 speed;
    f32 dist;
    f32 t;
    f32 angle;
    s32 sound;

    if (obj->speed > D_800ACE68) {
        func_0020E810(&cur, &obj->dir);
        speed = func_00298470(obj->vel.x * obj->vel.x + obj->vel.y * obj->vel.y + obj->vel.z * obj->vel.z);
        player = obj->owner->player;
        home.x = -26.623999f;
        home.y = -20.48f;
        home.z = -143.36f;
        func_00253F90(player, &home);
        func_0020EF2C(&toCatch, &home, &obj->pos);
        dist = func_00298470(toCatch.x * toCatch.x + toCatch.y * toCatch.y + toCatch.z * toCatch.z);
        if (dist <= D_800ACE6C + 30.72f) {
            if (dist < D_800ACE6C * (1.0f / 3) || obj->speed >= 65.0f) {
                obj->pos = home;
                obj->caught = 1;
                if (player->weapon == 0x15) {
                    player->f4D8 = 0.73303837f;
                }
                return;
            }
            toHome = toCatch;
        } else {
            aim.x = -102.399994f;
            aim.y = func_002671B4(obj->owner) * 0.5f;
            aim.z = -D_800ACE6C;
            func_00253F90(player, &aim);
            func_0020EF2C(&toHome, &aim, &obj->pos);
            func_00298470(toHome.x * toHome.x + toHome.y * toHome.y + toHome.z * toHome.z);
        }
        func_0020F040(&toHome);
        func_0020E810(&want, &toHome);
        func_00211074(&want);
        func_00211074(&cur);
        func_0020F040(&toHome);
        facing = obj->dir;
        func_0020F040(&facing);
        angle = func_0021153C(toHome.x * facing.x + toHome.y * facing.y + toHome.z * facing.z);
        t = (angle < 0.26179942f) ? 1.0f : 0.5f;
        func_0020DCC8(&rot, t, &cur, &want);
        func_00211114(&rot, mtx);
        forward.x = 0.0f;
        forward.y = 0.0f;
        forward.z = 1.0f;
        func_0020F85C(mtx, &forward, &obj->dir);
        func_0020EF60(&obj->vel, &obj->dir, speed);
    } else {
        loop = obj->loop;
        sound = 0x1B3;
        if (loop != NULL) {
            if (loop->sound == sound && loop->state != -1) {
                goto playing;
            }
            func_00232B40(obj);
        }
        obj->loop = func_00275C6C(func_00275F7C(), 0x1B3, &obj->pos, &obj->pos, -1);
    }
playing:
    Missile_Accelerate(obj);
    func_00229618(obj, 1.0f);
}
