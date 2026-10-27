#include "common.h"

typedef struct WeaponInfo WeaponInfo;

typedef struct MenuSlot {
    /* 0x00 */ s32 weapon;
    /* 0x04 */ WeaponInfo *info;
    /* 0x08 */ s32 available;
    /* 0x0C */ f32 angle;
    /* 0x10 */ s32 lift;
    /* 0x14 */ f32 scale;
} MenuSlot;

typedef struct WeaponMenu {
    /* 0x000 */ s32 state;
    /* 0x004 */ u8 pad4[8];
    /* 0x00C */ f32 spin;
    /* 0x010 */ u8 pad10[8];
    /* 0x018 */ s32 count;
    /* 0x01C */ MenuSlot slots[36];
    /* 0x37C */ s32 cursor;
} WeaponMenu;

typedef struct Player {
    /* 0x000 */ u8 pad0[0xB14];
    /* 0xB14 */ s16 nextWeapon;
} Player;

extern f32 D_800AE2D8;              /* slot scale */
extern WeaponInfo *D_800AF618[];

s32 func_0024E700(Player *player, s32 weapon);

/* Fills the weapon menu with the weapons flagged in `owned` (35 entries,
 * `rows` times over), spreading the slots evenly around the wheel. */
void func_0024BF1C(WeaponMenu *menu, s32 rows, s32 *owned, s32 perRow, Player *player) {
    s32 i;
    s32 n;
    s32 w;
    WeaponInfo *info;
    s32 ok;

    menu->count = 0;
    for (i = 0; i < perRow * rows; i++) {
        menu->slots[i].angle = -(i * (360.0f / (perRow * rows)) * 0.017453294f);
        menu->slots[i].lift = 0;
        menu->slots[i].scale = D_800AE2D8;
    }
    n = 0;
    for (i = 0; i < rows; i++) {
        for (w = 0; w < 35; w++) {
            if (owned[w] != 0) {
                info = D_800AF618[w];
                ok = func_0024E700(player, w) != 0;
                menu->slots[n].weapon = w;
                menu->slots[n].available = ok;
                menu->slots[n].info = info;
                if (menu->cursor == -1 && w == player->nextWeapon) {
                    menu->cursor = n;
                    menu->spin = menu->slots[n].angle;
                }
                n++;
                menu->count++;
            }
        }
    }
}
