#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 flags;   /* 1: usable on land, 2: usable under water */
} WeaponInfo;

typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ s32 flags;
    /* 0x144 */ u8 pad144[0x972 - 0x144];
    /* 0x972 */ s8 owned[0x24];
    /* 0x996 */ s16 weapon;
} Player;

extern WeaponInfo *D_800AF618[];   /* weapon table */
extern s32 D_80110058;             /* first weapon - 1 */
extern s32 D_8011005C;             /* weapon count */
extern u8 D_8011AAF1;
extern s32 func_0024E700(Player *player, s32 weapon);

/* The next weapon the player can switch to: the first usable one after the
 * current, else the last usable one before it (weapons 12 and 23 are never
 * picked going up, 12 never going down); 0x18 or 0 when there is none. */
s32 func_00257BA0(Player *player) {
    s32 i;
    s32 usable;

    if (player->weapon < D_8011005C) {
        for (i = player->weapon + 1; i < D_8011005C; i++) {
            if (func_0024E700(player, i) && player->owned[i] && i != 23 && i != 12) {
                if (player->flags & 0x80) {
                    usable = D_800AF618[i]->flags & 2;
                } else {
                    usable = D_800AF618[i]->flags & 1;
                }
                if (usable) {
                    return i;
                }
            }
        }
    }
    for (i = player->weapon - 1; D_80110058 < i; i--) {
        if (func_0024E700(player, i) && player->owned[i] && i != 12) {
            if (player->flags & 0x80) {
                usable = D_800AF618[i]->flags & 2;
            } else {
                usable = D_800AF618[i]->flags & 1;
            }
            if (usable) {
                return i;
            }
        }
    }
    if (D_8011AAF1) {
        return 0x18;
    }
    return 0;
}
