/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_6.CPP, func_80217D74_de, with Turok 2's layouts. */

#include "common.h"

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
    /* 0x004 */ u8 pad4[0x18];
    /* 0x01C */ MenuSlot slots[36];
    /* 0x37C */ s32 cursor;
    /* 0x380 */ u8 pad380[4];
    /* 0x384 */ s32 unk384;
    /* 0x388 */ s32 bothHeld;
    /* 0x38C */ s32 unk38C;
    /* 0x390 */ s32 hover;
} WeaponMenu;

/* A slot seen from the menu's base: menu + i * 0x18 (as Rage Wars' Block24). */
typedef struct MenuRow {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ union {
        s32 id;
        struct {
            s16 hi;
            s16 lo;
        } half;
    } weapon;
    /* 0x20 */ u8 pad20[4];
    /* 0x24 */ s32 available;
} MenuRow;

typedef struct Controller {
    /* 0x00 */ u8 pad0[0xB0];
    /* 0xB0 */ u32 buttons;
} Controller;

typedef struct Player {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u32 flags;
    /* 0x144 */ u8 pad144[0x972 - 0x144];
    /* 0x972 */ s8 hasWeapon[0x996 - 0x972];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xA50 - 0x998];
    /* 0xA50 */ Controller *controller;
    /* 0xA54 */ u8 padA54[0xB14 - 0xA54];
    /* 0xB14 */ s16 nextWeapon;
    /* 0xB16 */ u8 padB16[0x1284 - 0xB16];
    /* 0x1284 */ s32 unk1284;
} Player;

typedef struct WeaponInfo {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ s16 alternate;
} WeaponInfo;

extern WeaponInfo *D_800AF618[];

s32 func_002548FC(Player *player);
void func_0024C2A0(WeaponMenu *menu, s32 open, Player *player);
s32 func_0024B88C(WeaponMenu *menu, Player *player);
void func_00275624(s32 sound);

/* Runs the player's weapon menu each frame: the menu buttons open or
 * rebuild it, and the slot under the stick becomes the hovered entry,
 * moving the cursor and choosing the slot's weapon (or its alternate the
 * player holds). */
void func_0024BAD4(WeaponMenu *menu, Player *player, u32 *input) {
    s32 open;
    s32 next;
    s32 slot;
    MenuRow *entry;
    s16 weapon;
    s16 alternate;

    open = player->controller->buttons & 0x8000;
    next = player->controller->buttons & 0x4000;
    if (func_002548FC(player) != 0) {
        open = next = 0;
    }
    if (menu->state == 0 || menu->state == 3) {
        if (!open && !next) {
            return;
        }
        func_0024C2A0(menu, open, player);
        player->unk1284 = 1;
        menu->state = 1;
    } else {
        if (menu->unk38C != 0) {
            if (!(player->flags & 0x80)) {
                goto toggle;
            }
        } else if (player->flags & 0x80) {
        toggle:
            func_0024C2A0(menu, open, player);
        }
        if (open && next && menu->bothHeld == 0) {
            if (menu->unk384 != 0) {
                func_0024C2A0(menu, 0, player);
            } else {
                func_0024C2A0(menu, 1, player);
            }
            menu->bothHeld = 1;
        } else if (!open || !next) {
            menu->bothHeld = 0;
            if (menu->unk384 != 0) {
                if (!open) {
                    goto checkNext;
                }
            } else {
                if (!open) {
                    goto stick;
                }
                goto call;
            checkNext:
                if (next) {
                call:
                    func_0024C2A0(menu, open, player);
                }
            }
        }
    }
stick:
    slot = func_0024B88C(menu, player);
    if (slot == menu->hover) {
        return;
    }
    if (slot != -1) {
        entry = (MenuRow *) ((u8 *) menu + slot * sizeof(MenuSlot));
        if (entry->available == 0) {
            func_00275624(0x1455);
            return;
        }
        if (entry->weapon.id == -1) {
            menu->hover = -1;
            return;
        }
        if (slot != menu->cursor) {
            menu->cursor = slot;
        }
        weapon = entry->weapon.half.lo;
        if (weapon == player->weapon) {
            menu->hover = slot;
            return;
        }
        player->nextWeapon = entry->weapon.half.lo;
        if (*input & 0x80) {
            menu->hover = slot;
            return;
        }
        alternate = D_800AF618[weapon]->alternate;
        if (alternate != -1 && player->hasWeapon[alternate] != 0) {
            entry->weapon.id = alternate;
        }
    }
    menu->hover = slot;
}
