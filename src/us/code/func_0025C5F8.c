#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct Object Object;
typedef struct AI AI;

/* A mode's callbacks and its animations for the three mode slots. */
typedef struct {
    /* 0x00 */ void (*enter)(Object *obj, AI *ai);
    /* 0x04 */ void (*update)(Object *obj, AI *ai);
    /* 0x08 */ s32 anim[3];
} ModeHandler;

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ void (*update)(Object *obj, AI *ai);
} Behaviour;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 id;
} Sound;

struct AI {
    /* 0x000 */ u32 flags;
    /* 0x004 */ s32 active;
    /* 0x008 */ u8 pad8[0x2C - 0x8];
    /* 0x02C */ s32 anim;
    /* 0x030 */ Behaviour *behaviour;
    /* 0x034 */ s8 state;
    /* 0x035 */ u8 pad35[0x54 - 0x35];
    /* 0x054 */ f32 homeX;
    /* 0x058 */ u8 pad58[4];
    /* 0x05C */ f32 homeZ;
    /* 0x060 */ u8 pad60[0x80 - 0x60];
    /* 0x080 */ Object *target;
    /* 0x084 */ u8 pad84[0x9C - 0x84];
    /* 0x09C */ f32 posX;
    /* 0x0A0 */ u8 padA0[4];
    /* 0x0A4 */ f32 posZ;
    /* 0x0A8 */ u8 padA8[0xCB - 0xA8];
    /* 0x0CB */ s8 age;
    /* 0x0CC */ u8 padCC[0xD0 - 0xCC];
    /* 0x0D0 */ f32 burnTime;
    /* 0x0D4 */ u8 padD4[0xEC - 0xD4];
    /* 0x0EC */ Sound *sound;
    /* 0x0F0 */ u8 padF0[0x120 - 0xF0];
    /* 0x120 */ f32 timer120;
    /* 0x124 */ f32 idleTime;
    /* 0x128 */ u8 pad128[0x130 - 0x128];
    /* 0x130 */ s8 mode[2];
    /* 0x132 */ s8 mode2;
    /* 0x133 */ s8 modeIndex;
};

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u32 flags;
} ActorDef;

struct Object {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0xD4 - 0x18];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x140 */ AI ai;
    /* 0x274 */ u8 pad274[0x27C - 0x274];
    /* 0x27C */ u32 fxFlags;
};

extern f32 D_800B6D28;          /* frame time */
extern f32 D_800B6D2C;          /* time scale */
extern u8 D_80119870[];         /* players */
extern ModeHandler *D_800B1790[];

void *func_002532EC(u8 *players, Vec3f pos);
f32 func_002119FC(f32 lo, f32 hi);
void func_00243414(Object *obj, AI *ai, s32 state);
void func_0025C338(Object *obj, AI *ai);
void func_0025AD90(Object *obj, AI *ai, f32);
void func_00275544(s16 sound, Vec3f pos, s32, s32);
void func_00246860(Object *obj, AI *ai, s32 sound);
void func_00246918(Object *obj, AI *ai);

/* AI update: age, scale the frame time, run the current mode handler (its
 * enter callback when the animation changes), count down the timers, start
 * idle animations and the swim/land switches, run the behaviour, and keep
 * the burning effect and its sound going while burnTime lasts. */
void func_0025C5F8(Object *obj, AI *ai) {
    Object *target;
    ModeHandler *handler;
    s32 index;
    s32 mode;
    s32 anim;
    s32 state;
    s32 swimming;

    if (!(obj->flags & 0x2000)) {
        obj->flags &= ~0x100;
        return;
    }
    target = ai->target;
    if (target != NULL && *(s32 *)target->def == 1 && (target->ai.state == 0x3C || !(target->flags & 0x100))) {
        ai->flags &= ~0x20000000;
        ai->flags &= ~0x40000000;
    }
    if (ai->age < 100) {
        ai->age++;
    }
    D_800B6D28 *= D_800B6D2C;
    func_002532EC(D_80119870, obj->pos);
    mode = ai->mode[ai->modeIndex];
    index = ai->modeIndex;
    handler = D_800B1790[mode];
    swimming = mode == 2;
    if (handler != NULL) {
        switch (index) {
            case 0:
                anim = handler->anim[0];
                break;
            case 1:
                anim = handler->anim[1];
                break;
            case 2:
                anim = handler->anim[2];
                break;
            default:
                anim = 0;
                break;
        }
        if (ai->anim != anim) {
            ai->anim = anim;
            if (handler->enter != NULL) {
                handler->enter(obj, ai);
            }
        }
        if (handler->update != NULL) {
            handler->update(obj, ai);
        }
    }
    if (!(ai->state == 0x11 || ai->state == 0x12)) {
        ai->timer120 -= D_800B6D28;
        if (ai->timer120 < 0.0f) {
            ai->timer120 = 0.0f;
        }
    }
    index = ai->modeIndex;
    if (obj->def->flags & 0x40) {
        ai->idleTime -= D_800B6D28;
        if (ai->idleTime < 0.0f) {
            ai->idleTime = 0.0f;
        }
        if (ai->active && ai->idleTime <= 0.0f) {
            state = ai->state;
            if (state == 4 || state == 0xE || state == 0xF || state == 0x10 || state == 0x11 || state == 0x12
                || state == 0xA) {
                ai->idleTime = func_002119FC(75.0f, 150.0f);
                func_00243414(obj, ai, 0x17);
            }
        }
    }
    func_0025C338(obj, ai);
    if (ai->active) {
        if (index != 2 && (ai->flags & 0x80) && ai->state != 0x27 && ai->state != 0x29 && ai->mode2 != 0) {
            if (index == 0) {
                func_00243414(obj, ai, 0x27);
            } else if (index == 1) {
                func_00243414(obj, ai, 0x29);
            }
        }
        if (ai->active && index == 2 && !(ai->flags & 0x80) && ai->state != 0x2A && ai->state != 0x2B
            && ai->mode[0] != 0) {
            func_00243414(obj, ai, 0x2A);
        }
    }
    if (ai->behaviour != NULL && ai->behaviour->update != NULL) {
        ai->behaviour->update(obj, ai);
    }
    if (ai->active && swimming) {
        ai->homeX = ai->posX;
        ai->homeZ = ai->posZ;
        obj->flags &= ~0x20000;
    }
    func_0025AD90(obj, ai, 0.0f);
    if (ai->burnTime > 0.0f) {
        ai->burnTime -= D_800B6D28;
        obj->fxFlags |= 2;
        func_00275544(0x3D1, obj->pos, 0, -1);
        if (ai->active) {
            func_00246860(obj, ai, 0x3D2);
        } else if (ai->sound != NULL && ai->sound->id != 0x3D2) {
            func_00246918(obj, ai);
        }
    } else {
        obj->fxFlags &= ~2;
    }
    if (ai->state == 6) {
        obj->flags |= 0x800000;
    } else {
        obj->flags &= ~0x800000;
    }
}
