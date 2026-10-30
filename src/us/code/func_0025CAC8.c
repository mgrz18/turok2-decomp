#include "common.h"

/* AI hit callback: a projectile (or its owner) close enough lengthens the
 * wait and becomes the last-heard position; then the projectile type picks a
 * reaction animation (flinch, burn, freeze, shock...) the definition allows. */

typedef struct { f32 x, y, z; } Vec3;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x2D - 0x4];
    /* 0x2D */ u8 flinchDelay;
} ActorInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ ActorInfo info;
} ActorDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3 pos;
    /* 0x010 */ u8 pad10[0x14 - 0x10];
    /* 0x014 */ ActorDef *def;
} Actor;

typedef struct Projectile {
    /* 0x000 */ u8 pad0[0x2];
    /* 0x002 */ u16 type;
    /* 0x004 */ Vec3 pos;
    /* 0x010 */ u8 pad10[0x10C - 0x10];
    /* 0x10C */ struct Projectile *owner;
    /* 0x110 */ u8 pad110[0x1BC - 0x110];
    /* 0x1BC */ f32 charge;
} Projectile;

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ s32 unk4;
    /* 0x008 */ u8 pad8[0x10 - 0x8];
    /* 0x010 */ u8 flinchTimer;
    /* 0x011 */ u8 pad11[0x34 - 0x11];
    /* 0x034 */ s8 state;
    /* 0x035 */ u8 pad35[0x44 - 0x35];
    /* 0x044 */ f32 wait;
    /* 0x048 */ u8 pad48[0x64 - 0x48];
    /* 0x064 */ f32 reactTime;
    /* 0x068 */ u8 pad68[0xC8 - 0x68];
    /* 0x0C8 */ u8 intensity;
    /* 0x0C9 */ u8 padC9[0xD0 - 0xC9];
    /* 0x0D0 */ f32 unkD0;
    /* 0x0D4 */ u8 padD4[0xE0 - 0xD4];
    /* 0x0E0 */ Vec3 hitNormal;
    /* 0x0EC */ u8 padEC[0x130 - 0xEC];
    /* 0x130 */ s8 mode[3];
    /* 0x133 */ s8 modeIndex;
} CAI;

typedef struct {
    /* 0x00 */ s32 hit;
    /* 0x04 */ u8 pad4[0x7D - 0x4];
    /* 0x7D */ u8 surface;
    /* 0x7E */ u8 pad7E[0xE4 - 0x7E];
    /* 0xE4 */ Vec3 normal;
} CollisionResult;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

extern Vec3 D_800B06AC;
extern CollisionResult D_800F1CE0;

extern f32 func_0020F6BC(Vec3 *a, Vec3 *b);
extern void func_00243414(Actor *actor, CAI *ai, s32 anim);
extern void func_00275544(s16 sound, Vec3 pos, s32, s32);

/* A noise at pos within 3072 units lengthens the wait and is remembered. */
static __inline__ void hearNoise(Actor *actor, CAI *ai, Vec3 *pos) {
    s32 mode;
    f32 dist;
    f32 add;

    mode = ai->mode[ai->modeIndex];
    if (mode >= 0) {
        if (mode < 3) {
            dist = func_0020F6BC(&actor->pos, pos);
            if (dist < 3072.0f) {
                add = (3072.0f - dist) * 0.048828125f;
                if (ai->wait < add) {
                    ai->wait += add;
                    if (add < ai->wait) {
                        ai->wait = add;
                    }
                    D_800B06AC = *pos;
                }
            }
        }
    }
}

/* level and delay are function-wide variables shared by the three reactions
 * (their register choices depend on it). */
void func_0025CAC8(Actor *actor, CAI *ai, Projectile *proj) {
    ActorInfo *info;
    Projectile *owner;
    s32 v;
    f32 t;
    f32 level;
    f32 delay;

    info = &actor->def->info;
    if (ai->unk4 == 0) {
        return;
    }
    owner = proj->owner;
    if (owner != NULL) {
        hearNoise(actor, ai, &owner->pos);
    } else {
        hearNoise(actor, ai, &proj->pos);
    }
    switch (proj->type) {
    case 3:
        if ((info->flags & 0x200) && ai->state != 0x3D) {
            if (--ai->flinchTimer == 0) {
                ai->flinchTimer += info->flinchDelay;
                if (ai->state == 0x1D) {
                    v = ai->intensity;
                    v += 10;
                    if (v >= 0x100) {
                        v = 0xFF;
                    }
                    ai->intensity = v;
                    level = ai->intensity;
                    level *= (1.0f / 255);
                    delay = level * -120.0f + 240.0f;
                    ai->reactTime += delay;
                } else {
                    func_00243414(actor, ai, 0x1D);
                }
            }
        }
        break;
    case 0x128:
        proj->charge = 0.0f;
    case 4:
    case 0x1CC:
    case 0x1CD:
    case 0x1CE:
    case 0x1CF:
    case 0x1D0:
    case 0x1D1:
    case 0x1D2:
        if ((info->flags & 0x100) && ai->state != 0x3D) {
            if (ai->state == 0x20) {
                v = ai->intensity;
                v += 10;
                if (v >= 0x100) {
                    v = 0xFF;
                }
                ai->intensity = v;
                ai->reactTime = MIN(ai->reactTime += 15.0f, 300.0f);
            } else {
                func_00243414(actor, ai, 0x20);
                ai->reactTime = proj->charge * 300.0f + 15.0f;
            }
        }
        break;
    case 10:
        if (info->flags & 0x80) {
            if (ai->state == 0x1F) {
                v = ai->intensity;
                v += 10;
                if (v >= 0x100) {
                    v = 0xFF;
                }
                ai->intensity = v;
                level = ai->intensity;
                level *= (1.0f / 255);
                delay = level * -45.0f + 105.0f;
                ai->reactTime += delay;
            } else {
                func_00243414(actor, ai, 0x1F);
            }
        }
        break;
    case 11:
    case 0x129:
        if (D_800F1CE0.hit != 0 && (D_800F1CE0.surface & 3) == 1) {
            if (info->flags & 0x400) {
                func_00275544(0x1AB, actor->pos, 0, -1);
                if (ai->state == 0x21) {
                    v = ai->intensity;
                    v += 20;
                    if (v >= 0x100) {
                        v = 0xFF;
                    }
                    ai->intensity = v;
                    level = ai->intensity;
                    level *= (1.0f / 255);
                    delay = level * -15.0f + 75.0f;
                    ai->reactTime += delay;
                } else {
                    ai->hitNormal = D_800F1CE0.normal;
                    func_00243414(actor, ai, 0x21);
                }
            } else if (info->flags & 0x2000) {
                func_00275544(0x213, actor->pos, 0, -1);
                func_00243414(actor, ai, 0x22);
            }
        }
        break;
    case 0x14:
        t = ai->unkD0;
        t += 30.0f;
        if (!(t <= 120.0f)) {
            t = 120.0f;
        }
        ai->unkD0 = t;
        break;
    case 0:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x15:
    case 0x17:
        break;
    }
}
