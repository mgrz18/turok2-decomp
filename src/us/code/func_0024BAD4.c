/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_6.CPP, func_80217D74_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

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

s32 func_002548FC(CPlayer *player);
void func_0024C2A0(WeaponMenu *menu, s32 open, CPlayer *player);
s32 func_0024B88C(WeaponMenu *menu, CPlayer *player);
void func_00275624(s32 sound);

/* Runs the player's weapon menu each frame: the menu buttons open or
 * rebuild it, and the slot under the stick becomes the hovered entry,
 * moving the cursor and choosing the slot's weapon (or its alternate the
 * player holds). */
void func_0024BAD4(WeaponMenu *menu, CPlayer *player, u32 *input) {
    s32 open;
    s32 next;
    s32 slot;
    MenuRow *entry;
    s16 weapon;
    s16 alternate;

    open = ((Controller *)player->m_ContState.m_pContState)->buttons & 0x8000;
    next = ((Controller *)player->m_ContState.m_pContState)->buttons & 0x4000;
    if (func_002548FC(player) != 0) {
        open = next = 0;
    }
    if (menu->state == 0 || menu->state == 3) {
        if (!open && !next) {
            return;
        }
        func_0024C2A0(menu, open, player);
        player->m_bHeadMovementDisabled = 1;
        menu->state = 1;
    } else {
        if (menu->unk38C != 0) {
            if (!(player->m_CharacterActor.m_AI.m_dwFlags & 0x80)) {
                goto toggle;
            }
        } else if (player->m_CharacterActor.m_AI.m_dwFlags & 0x80) {
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
        if (weapon == player->m_Data.m_CurrentWeapon) {
            menu->hover = slot;
            return;
        }
        player->m_RequestedWeapon = entry->weapon.half.lo;
        if (*input & 0x80) {
            menu->hover = slot;
            return;
        }
        alternate = D_800AF618[weapon]->alternate;
        if (alternate != -1 && player->m_Data.m_WeaponList[alternate] != 0) {
            entry->weapon.id = alternate;
        }
    }
    menu->hover = slot;
}
