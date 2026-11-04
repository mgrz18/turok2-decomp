#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s16 nextWeapon;
} WeaponDef;

typedef struct {
    /* 0x0000 */ u8 pad0[0x972];
    /* 0x0972 */ s8 weapons[1];       /* CPlayer.m_Data.m_WeaponList */
    /* 0x0973 */ u8 pad973[0xA64 - 0x973];
    /* 0x0A64 */ u32 buttons;
    /* 0x0A68 */ u32 pressed;
    /* 0x0A6C */ u8 padA6C[0xB14 - 0xA6C];
    /* 0x0B14 */ s16 weapon;
    /* 0x0B16 */ u8 padB16[0xCD0 - 0xB16];
    /* 0x0CD0 */ s32 unkCD0;
    /* 0x0CD4 */ u8 padCD4[0x1050 - 0xCD4];
    /* 0x1050 */ s32 unk1050;
} Player;

extern WeaponDef *D_800AF618[];
extern s32 D_8011AAF1; /* a u8 game-mode flag; D_8011AAA0 is addressed through it */

#define D_8011AAA0 (*(s32 *) ((u8 *) &D_8011AAF1 - 0x51))

void func_002573FC(Player *arg0) {
    s16 temp_a1;
    WeaponDef *temp_v0;

    if ((*(u8 *) &D_8011AAF1 == 0) && ((arg0->unk1050 == 0) || (arg0->unkCD0 == 0)) && ((D_8011AAA0 == 0) || !(arg0->buttons & 0x20)) && (arg0->pressed & 0x200)) {
        temp_v0 = D_800AF618[arg0->weapon];
        temp_a1 = temp_v0->nextWeapon;
        if ((temp_a1 != -1) && (arg0->weapons[temp_a1] != 0)) {
            arg0->weapon = (s16) (u16) temp_v0->nextWeapon;
        }
    }
}
