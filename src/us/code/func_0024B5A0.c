#include "common.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

typedef struct MenuSlot {
    /* 0x00 */ s32 weapon;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 available;
    /* 0x0C */ f32 angle;
    /* 0x10 */ f32 lift;
    /* 0x14 */ f32 scale;
} MenuSlot;

typedef struct WeaponMenu {
    /* 0x000 */ s32 state;
    /* 0x004 */ f32 timer;
    /* 0x008 */ u8 pad8[4];
    /* 0x00C */ f32 spin;
    /* 0x010 */ s32 direction;
    /* 0x014 */ u8 pad14[4];
    /* 0x018 */ s32 count;
    /* 0x01C */ MenuSlot slots[36];
    /* 0x37C */ s32 cursor;
} WeaponMenu;

typedef struct Player {
    /* 0x000 */ u8 pad0[0xA68];
    /* 0xA68 */ u32 buttons;
    /* 0xA6C */ u8 padA6C[0xB14 - 0xA6C];
    /* 0xB14 */ s16 weapon;
} Player;

extern f32 D_800B6D28;      /* frame step */

s32 func_0024E700(Player *player, s32 weapon);
s32 func_002548FC(Player *player);
s32 func_00254F88(WeaponMenu *menu, Player *player);
void func_0024C47C(WeaponMenu *menu, Player *player);
void func_00210EF0(f32 *angle);

/* Runs the weapon menu: marks the weapons the player can use, steps the
 * cursor with the next/previous weapon buttons and turns the wheel towards
 * the selected slot. */
void func_0024B5A0(WeaponMenu *menu, Player *player) {
    s32 step;
    s32 i;
    s32 dir;
    f32 from;
    f32 to;
    f32 diff;
    f32 turn;

    dir = 0;
    if (menu->state == 1 || menu->state == 2) {
        for (i = 0; i < menu->count; i++) {
            if (func_0024E700(player, menu->slots[i].weapon) != 0) {
                menu->slots[i].available = 1;
            } else {
                menu->slots[i].available = 0;
            }
        }
    }
    if (func_002548FC(player) != 0) {
        dir = 0;
    } else if (player->buttons & 0x4000) {
        dir = 1;
    } else if (player->buttons & 0x8000) {
        dir = -1;
    }
    if ((menu->state == 0 && dir != 0)
        || (func_00254F88(menu, player) != 0 && menu->state != 0)
        || (menu->cursor != -1 && menu->slots[menu->cursor].weapon != player->weapon)) {
        func_0024C47C(menu, player);
    }
    if (dir != 0) {
        do {
            menu->cursor += dir;
            if (menu->cursor >= menu->count) {
                menu->cursor = 0;
            } else if (menu->cursor < 0) {
                menu->cursor = menu->count - 1;
            }
        } while (menu->slots[menu->cursor].available == 0);
        player->weapon = menu->slots[menu->cursor].weapon;
        menu->direction = -dir;
        menu->timer = 45.0f;
        if (menu->state == 0 || menu->state == 3) {
            menu->state = 1;
        }
    }
    if (menu->state != 0) {
        from = menu->spin;
        to = menu->slots[menu->cursor].angle;
        step = menu->direction;
        func_00210EF0(&from);
        func_00210EF0(&to);
        if (step > 0) {
            if (to < from) {
                to += 6.283186f;
            }
        } else if (to > from) {
            to -= 6.283186f;
        }
        do {
            diff = to - from;
            turn = diff * 0.25f * D_800B6D28;
            if (ABS(turn) > ABS(diff)) {
                turn = diff;
            }
        } while (0);
        menu->spin += turn;
    }
}
