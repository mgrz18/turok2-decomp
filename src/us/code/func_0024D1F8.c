#include "common.h"

typedef struct WeaponInfo {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u32 flags;
} WeaponInfo;

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x288];
    /* 0x0288 */ u8 unk288[0x3C8 - 0x288];
    /* 0x03C8 */ u8 unk3C8[0x996 - 0x3C8];
    /* 0x0996 */ s16 weapon;
    /* 0x0998 */ s16 savedWeapon[3];
    /* 0x099E */ u8 pad99E[0xA08 - 0x99E];
    /* 0x0A08 */ u16 state;
    /* 0x0A0A */ u8 padA0A[0xA1C - 0xA0A];
    /* 0x0A1C */ u32 flags;
    /* 0x0A20 */ u8 padA20[0xB14 - 0xA20];
    /* 0x0B14 */ s16 nextWeapon;
    /* 0x0B16 */ u8 padB16[0x129C - 0xB16];
    /* 0x129C */ s32 lastWeaponSet;
    /* 0x12A0 */ s32 weaponSet;
} Player;

extern WeaponInfo *D_800AF618[];

void func_00258668(void *a, void *b);

/* Picks the player's weapon set (0 on foot, 1 in states 10-11, 2 with
 * flag 0x8000); when it changes, remembers the weapon held in the old set
 * if it belongs there and queues the new set's weapon. */
void func_0024D1F8(Player *player) {
    s32 keep;
    s16 weapon;

    player->lastWeaponSet = player->weaponSet;
    if ((u32) (player->state - 10) < 2) {
        player->weaponSet = 1;
    } else if (player->flags & 0x8000) {
        player->weaponSet = 2;
    } else {
        player->weaponSet = 0;
    }
    if (player->lastWeaponSet == player->weaponSet) {
        return;
    }
    weapon = player->weapon;
    switch (player->lastWeaponSet) {
    case 0:
        if (!(D_800AF618[weapon]->flags & 1)) {
            goto no;
        }
        keep = 1;
        break;
    case 1:
        if (!(D_800AF618[weapon]->flags & 2)) {
            goto no;
        }
        keep = 1;
        break;
    case 2:
        if (weapon != 19) {
            goto no;
        }
        keep = 1;
        break;
    default:
    no:
        keep = 0;
        break;
    }
    if (keep) {
        player->savedWeapon[player->lastWeaponSet] = player->weapon;
    }
    player->nextWeapon = player->savedWeapon[player->weaponSet];
    if (player->nextWeapon == 19) {
        func_00258668(player->unk288, player->unk3C8);
    }
}
