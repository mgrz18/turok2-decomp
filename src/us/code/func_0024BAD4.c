/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_6.CPP, func_80217D74_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

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
void func_0024C2A0(CBarrel *menu, s32 open, CPlayer *player);
s32 func_0024B88C(CBarrel *menu, CPlayer *player);
void func_00275624(s32 sound);

/* Runs the player's weapon menu each frame: the menu buttons open or
 * rebuild it, and the slot under the stick becomes the hovered entry,
 * moving the cursor and choosing the slot's weapon (or its alternate the
 * player holds). */
void func_0024BAD4(CBarrel *menu, CPlayer *player, u32 *input) {
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
    if (menu->m_State == 0 || menu->m_State == 3) {
        if (!open && !next) {
            return;
        }
        func_0024C2A0(menu, open, player);
        player->m_bHeadMovementDisabled = 1;
        menu->m_State = 1;
    } else {
        if (menu->m_bUnderwater != 0) {
            if (!(player->m_CharacterActor.m_AI.m_dwFlags & 0x80)) {
                goto toggle;
            }
        } else if (player->m_CharacterActor.m_AI.m_dwFlags & 0x80) {
        toggle:
            func_0024C2A0(menu, open, player);
        }
        if (open && next && menu->m_bBothHeld == 0) {
            if (menu->m_Group != 0) {
                func_0024C2A0(menu, 0, player);
            } else {
                func_0024C2A0(menu, 1, player);
            }
            menu->m_bBothHeld = 1;
        } else if (!open || !next) {
            menu->m_bBothHeld = 0;
            if (menu->m_Group != 0) {
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
    if (slot == menu->m_Hover) {
        return;
    }
    if (slot != -1) {
        entry = (MenuRow *) ((u8 *) menu + slot * sizeof(CBarrelItem));
        if (entry->available == 0) {
            func_00275624(0x1455);
            return;
        }
        if (entry->weapon.id == -1) {
            menu->m_Hover = -1;
            return;
        }
        if (slot != menu->m_nCursor) {
            menu->m_nCursor = slot;
        }
        weapon = entry->weapon.half.lo;
        if (weapon == player->m_Data.m_CurrentWeapon) {
            menu->m_Hover = slot;
            return;
        }
        player->m_RequestedWeapon = entry->weapon.half.lo;
        if (*input & 0x80) {
            menu->m_Hover = slot;
            return;
        }
        alternate = D_800AF618[weapon]->alternate;
        if (alternate != -1 && player->m_Data.m_WeaponList[alternate] != 0) {
            entry->weapon.id = alternate;
        }
    }
    menu->m_Hover = slot;
}
