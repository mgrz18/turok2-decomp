/* NON-MATCHING: 12 of 71 words differ: the clearing loops; the ROM indexes from the player base (sh 0x934(v1)) with the counter in v0, these pointer loops address from the element */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x48];
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
} WeaponDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x3F4];
    /* 0x3F4 */ s32 unk3F4;
    /* 0x3F8 */ u8 pad3F8[0x4D0 - 0x3F8];
    /* 0x4D0 */ s32 unk4D0;
    /* 0x4D4 */ u8 pad4D4[0x520 - 0x4D4];
    /* 0x520 */ s32 character;
    /* 0x524 */ u8 pad524[0x934 - 0x524];
    /* 0x934 */ s16 ammo[0x1F];
    /* 0x972 */ s8 owned[0x23];
    /* 0x995 */ u8 pad995;
    /* 0x996 */ s16 weapon;
    /* 0x998 */ s16 weapon2;
    /* 0x99A */ s16 weapon3;
    /* 0x99C */ s16 weapon4;
    /* 0x99E */ u8 pad99E[0xB14 - 0x99E];
    /* 0xB14 */ s16 queuedWeapon;
} ArmsPlayer;

extern u8 D_8011AAF1;            /* multiplayer */
extern WeaponDef *D_800AF618[];  /* the weapons */

/* Takes every weapon and all ammo away, then gives the starting kit: in
 * multiplayer by character (10 gets weapon 0x22), otherwise the
 * knife-and-bow set with 20 rounds. */
void func_00248A8C(ArmsPlayer *p) {
    s32 i;
    s16 weapon;

    {
        s16 *a = &p->ammo[0x1E];
        s8 *o;

        for (i = 0x1E; i >= 0; i--) {
            *a-- = 0;
        }
        o = &p->owned[0x22];
        for (i = 0x22; i >= 0; i--) {
            *o-- = 0;
        }
    }
    if (D_8011AAF1) {
        if (p->character == 10) {
            weapon = 0x22;
            p->owned[0x22] = 1;
            p->weapon2 = weapon;
            p->weapon3 = weapon;
        } else {
            weapon = 0x18;
            p->owned[0x18] = 1;
            p->owned[0x20] = 1;
            p->weapon3 = 0x20;
            p->weapon2 = weapon;
        }
        p->weapon4 = -1;
        p->queuedWeapon = weapon;
        p->weapon = weapon;
    } else {
        p->owned[0] = 1;
        p->owned[2] = 1;
        p->owned[0x17] = 1;
        p->ammo[0] = 0x14;
        p->ammo[0x15] = 1;
        p->ammo[0x17] = 1;
        p->queuedWeapon = 0;
        p->weapon = 0;
        p->weapon2 = 0;
        p->weapon3 = 0;
        p->weapon4 = 0x13;
    }
    if (D_8011AAF1) {
        p->unk3F4 = D_800AF618[p->queuedWeapon]->unk48;
        p->unk4D0 = D_800AF618[p->weapon]->unk4C;
    }
}
