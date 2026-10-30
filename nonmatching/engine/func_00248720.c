/* NON-MATCHING: 215 of 233 words differ: readable draft from the asm (m2c fails on it); control flow of the ammo/animation switch and the projectile choice not yet tuned */
#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;
typedef struct { f32 x, y, z, w; } Quat;

typedef struct {
    /* 0x0 */ s16 index;            /* into the player's ammo */
    /* 0x2 */ s16 max;
    /* 0x4 */ s16 projectile;
    /* 0x6 */ u16 cost;
} AmmoInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ AmmoInfo *ammo[3];
} WeaponDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ Vec3f pos;
} Camera;

typedef struct {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ Vec3f pos;
    /* 0x0010 */ u8 pad10[8];
    /* 0x0018 */ Vec3f vel;
    /* 0x0024 */ u8 pad24[0xD8 - 0x24];
    /* 0x00D8 */ f32 charge;
    /* 0x00DC */ u8 padDC[0xE2 - 0xDC];
    /* 0x00E2 */ u8 firing;
    /* 0x00E3 */ u8 padE3[0x140 - 0xE3];
    /* 0x0140 */ u32 aiFlags;
    /* 0x0144 */ u8 pad144[0x51C - 0x144];
    /* 0x051C */ Camera *camera;
    /* 0x0520 */ u8 pad520[0x934 - 0x520];
    /* 0x0934 */ s16 ammo[0x1F];
    /* 0x0972 */ s8 owned[0x23];
    /* 0x0995 */ u8 pad995;
    /* 0x0996 */ s16 weapon;
    /* 0x0998 */ u8 pad998[0xA08 - 0x998];
    /* 0x0A08 */ u16 state;
    /* 0x0A0A */ u8 padA0A[0xB2C - 0xA0A];
    /* 0x0B2C */ s32 lockMode;
    /* 0x0B30 */ u8 padB30[8];
    /* 0x0B38 */ s32 lockTarget;
    /* 0x0B3C */ u8 padB3C[0xB7C - 0xB3C];
    /* 0x0B7C */ Quat aim;
    /* 0x0B8C */ s32 sniperMode;
    /* 0x0B90 */ u8 padB90[0xC04 - 0xB90];
    /* 0x0C04 */ s32 anim;
    /* 0x0C08 */ u8 padC08[0x1294 - 0xC08];
    /* 0x1294 */ f32 power;
} FirePlayer;

extern WeaponDef *D_800AF618[];  /* the weapons */
extern f32 D_800AE350;
extern f32 D_800ACE10[2];
extern u8 D_800F8928[];

Quat func_0021AE6C(FirePlayer *p);
AmmoInfo *func_0025398C(FirePlayer *p, s32 weapon);
void func_0022F350(void *list, FirePlayer *src, FirePlayer *owner, s32 a3, s32 target, s32 type,
                   Vec3f vel, Quat aim, Vec3f pos, s32 a40, s32 a44, s32 a48);

/* Whether the weapon fires the given projectile. */
static __inline__ s32 usesProjectile(s32 weapon, s32 type) {
    WeaponDef *def = D_800AF618[weapon];
    s32 i;

    for (i = 0; i < 3; i++) {
        if (def->ammo[i] == NULL) {
            return 0;
        }
        if (def->ammo[i]->projectile == type) {
            return 1;
        }
    }
    return 0;
}

/* Fires the player's weapon: spends the ammo, advances the firing
 * animation, picks the projectile (sniper and charge variants) and
 * launches it from the camera. */
void func_00248720(FirePlayer *p, s32 type) {
    Vec3f pos;
    Vec3f vel;
    Quat aim;
    Camera *camera = p->camera;
    AmmoInfo *info;
    s16 *ammo;
    s32 target;
    f32 f;

    if (p->state < 2) {
        return;
    }
    aim = func_0021AE6C(p);
    info = func_0025398C(p, p->weapon);
    if (info != NULL && usesProjectile(p->weapon, type)) {
        aim = p->aim;
        ammo = &p->ammo[info->index];
        *ammo -= info->cost;
        if (*ammo < 0) {
            *ammo = 0;
        }
        type = info->projectile;
        switch (p->anim) {
            case 1:
                p->anim = 0x1130;
                break;
            case 0xA28:
                p->anim = 0x1135;
                break;
            case 0x14:
                p->anim = 0x1144;
                break;
            case 0x1130:
            case 0x1135:
            case 0x1144:
                if (p->charge >= 3.0f) {
                    p->charge -= 3.0f;
                }
                break;
        }
        p->firing = 0;
    }
    D_800ACE10[0] = D_800AE350;
    target = 0;
    if (camera != NULL) {
        pos = camera->pos;
    } else {
        pos = p->pos;
    }
    vel = p->vel;
    if (p->lockMode == 2) {
        target = p->lockTarget;
    }
    D_800ACE10[1] = p->power;
    if (p->sniperMode) {
        switch (p->weapon) {
            case 10:
                type = 0x110;
                break;
            case 0x1B:
                type = 0x13A;
                break;
            case 3:
                if (type == 4) {
                    type = 0x111;
                } else if (type == 0x56) {
                    type = 0x126;
                }
                break;
        }
    }
    if (type == 4) {
        f = D_800ACE10[1] * 6.0f;
        type = 0x1D2;
        if (!(6.0f < f)) {
            type = f + 460.0f;
        }
    }
    func_0022F350(D_800F8928, p, p, 0, target, type, vel, aim, pos, 0, -1, (p->aiFlags & 0x80) ? 5 : 1);
    D_800ACE10[0] = D_800AE350 = p->power = D_800ACE10[1] = 1.0f;
}
