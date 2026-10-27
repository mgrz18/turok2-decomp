#include "common.h"

typedef struct WeaponInfo {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s16 alternate;
    /* 0x0E */ u8 padE[6];
    /* 0x14 */ u32 flags;
} WeaponInfo;

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
    /* 0x004 */ f32 radius;
    /* 0x008 */ u8 pad8[8];
    /* 0x010 */ s32 step;
    /* 0x014 */ s32 unk14;
    /* 0x018 */ s32 count;
    /* 0x01C */ MenuSlot slots[36];
    /* 0x37C */ s32 cursor;
} WeaponMenu;

typedef struct Player {
    /* 0x000 */ u8 pad0[0x3C8];
    /* 0x3C8 */ u32 flags;
    /* 0x3CC */ u8 pad3CC[0x972 - 0x3CC];
    /* 0x972 */ s8 hasWeapon[0x996 - 0x972];
    /* 0x996 */ u8 pad996[0xB14 - 0x996];
    /* 0xB14 */ s16 nextWeapon;
} Player;

extern WeaponInfo *D_800AF618[];
extern s32 D_80110058;
extern s32 D_8011005C;

void func_0024BF1C(WeaponMenu *menu, s32 rows, s32 *owned, s32 perRow, Player *player);

/* Opens the weapon menu: flags the weapons the player can pick (an owned
 * weapon usable in the current mode, unless it is the lower of a pair the
 * player owns both of), lays them out on one or two rings, then steps the
 * cursor by CREDITS.md	   Makefile	   docs		scripts  turok2.us.toml
Dockerfile	   README.md	   include	src	 us
Dockerfile.native  baserom.us.z64  nonmatching	test	 versions
LICENSE		   build	   references	tools until it rests on the queued weapon. */
void func_0024C0B4(WeaponMenu *menu, s32 dir, Player *player) {
    s32 owned[35];
    s32 count;
    s32 i;
    s32 range;
    s32 rows;
    s32 ok;
    WeaponInfo *info;
    s32 alt;

    count = 0;
    range = D_8011005C - D_80110058 + 1;
    for (i = 0; i < 35; i++) {
        info = D_800AF618[i];
        if (player->hasWeapon[i] == 0) {
            ok = 0;
        } else if (!((player->flags & 0x80) ? (info->flags & 2) : (info->flags & 1))) {
            ok = 0;
        } else {
            alt = info->alternate;
            ok = 1;
            if (player->hasWeapon[alt] != 0) {
                ok = !(i < alt);
            }
        }
        owned[i] = ok;
        if (ok) {
            count++;
        }
    }
    rows = range / count;
    if (rows < 2) {
        rows = 1;
        if (count < range * 0.75f) {
            rows = 2;
        }
    }
    func_0024BF1C(menu, rows, owned, count, player);
    menu->radius = 45.0f;
    if (menu->state == 0 || menu->state == 3) {
        menu->state = 1;
    }
    menu->step = -dir;
    menu->unk14 = 0;
    while (menu->slots[menu->cursor].weapon != player->nextWeapon) {
        menu->cursor += dir;
        if (menu->cursor < 0) {
            menu->cursor = menu->count - 1;
        } else if (menu->cursor >= menu->count) {
            menu->cursor = 0;
        }
    }
}
