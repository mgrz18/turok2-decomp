#include "common.h"

typedef struct {
    /* 0x0 */ s16 index;            /* into the player's ammo */
    /* 0x2 */ s16 max;
} AmmoInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ AmmoInfo *ammo[3];
} WeaponDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x524];
    /* 0x524 */ s32 health;         /* 8.8 */
    /* 0x528 */ u8 pad528[0x934 - 0x528];
    /* 0x934 */ s16 ammo[0x1F];
    /* 0x972 */ s8 owned[0x23];
    /* 0x995 */ u8 pad995[0x9A0 - 0x995];
    /* 0x9A0 */ s32 ammoFrom[6];
    /* 0x9B8 */ s32 healthFrom[6];
} AmmoPlayer;

extern WeaponDef *D_800AF618[];  /* the weapons */

s32 func_0026EFB8(s32 who);

/* A full refill from player who (1-6): kind 0 tops up the ammo of every
 * weapon the player owns, kind 1 the health to 100; returns whether
 * anything was given, and if so notes who gave it. */
s32 func_002484F8(AmmoPlayer *p, s32 kind, s32 who) {
    s32 given = 0;
    s32 slot;
    s32 any;
    s32 i;
    s32 j;
    WeaponDef *weapon;
    AmmoInfo *info;
    s16 *ammo;

    slot = func_0026EFB8(who);
    if (slot < 1 || slot > 6) {
        return given;
    }
    slot--;
    switch (kind) {
        case 0:
            any = 0;
            for (i = 0; i < 0x23; i++) {
                weapon = D_800AF618[i];
                if (p->owned[i] == 0) {
                    continue;
                }
                for (j = 0; j < 3; j++) {
                    info = weapon->ammo[j];
                    if (info == NULL) {
                        break;
                    }
                    ammo = &p->ammo[info->index];
                    if (*ammo < info->max) {
                        any = 1;
                    }
                    *ammo = (info->max < *ammo) ? *ammo : info->max;
                }
            }
            given = any;
            if (given) {
                p->ammoFrom[slot] = 1;
            }
            break;
        case 1:
            if (p->health < 0x6400) {
                p->health = 0x6400;
                given = 1;
            }
            if (given) {
                p->healthFrom[slot] = kind;
            }
            break;
        default:
            return given;
    }
    return given;
}
