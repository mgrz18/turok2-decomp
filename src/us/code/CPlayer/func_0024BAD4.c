/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_6.CPP, func_80217D74_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/player.h"
#include "structs/controller.h"
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


s32 CPlayer__IsInMatchEndState(CPlayer *player);
void CBarrel__LayoutEight(CBarrel *menu, s32 open, CPlayer *player);
s32 CBarrel__GetStickDirection(CBarrel *menu, CPlayer *player);
void Sound__Play2D(s32 sound);

/* Runs the player's weapon menu each frame: the menu buttons open or
 * rebuild it, and the slot under the stick becomes the hovered entry,
 * moving the cursor and choosing the slot's weapon (or its alternate the
 * player holds). */
void CBarrel__RunStick(CBarrel *menu, CPlayer *player, u32 *input) {
    s32 open;
    s32 next;
    s32 slot;
#ifndef TARGET_PORT
    MenuRow *entry;
#endif
    s16 weapon;
    s16 alternate;

    open = player->m_ContState.m_pContState->buttons & 0x8000;
    next = player->m_ContState.m_pContState->buttons & 0x4000;
    if (CPlayer__IsInMatchEndState(player) != 0) {
        open = next = 0;
    }
    if (menu->m_State == 0 || menu->m_State == 3) {
        if (!open && !next) {
            return;
        }
        CBarrel__LayoutEight(menu, open, player);
        player->m_bHeadMovementDisabled = 1;
        menu->m_State = 1;
    } else {
        if (menu->m_bUnderwater != 0) {
            if (!(player->m_CharacterActor.m_AI.m_dwFlags & 0x80)) {
                goto toggle;
            }
        } else if (player->m_CharacterActor.m_AI.m_dwFlags & 0x80) {
        toggle:
            CBarrel__LayoutEight(menu, open, player);
        }
        if (open && next && menu->m_bBothHeld == 0) {
            if (menu->m_Group != 0) {
                CBarrel__LayoutEight(menu, 0, player);
            } else {
                CBarrel__LayoutEight(menu, 1, player);
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
                    CBarrel__LayoutEight(menu, open, player);
                }
            }
        }
    }
stick:
    slot = CBarrel__GetStickDirection(menu, player);
    if (slot == menu->m_Hover) {
        return;
    }
#ifdef TARGET_PORT
    if (slot != -1) {
        /* the slot as CBarrelItem: MenuRow puts m_Items at the N64 offset 0x1C (0x20 on the host,
         * after the pointer-wide m_pInfo the item is 0x20 bytes) and reads the weapon as the low
         * half of a big-endian word (half.lo): on the host that is the high half, 0 */
        CBarrelItem *item = &menu->m_Items[slot];

        if (item->m_bAvailable == 0) {
            Sound__Play2D(0x1455);
            return;
        }
        if (item->m_Weapon == -1) {
            menu->m_Hover = -1;
            return;
        }
        if (slot != menu->m_nCursor) {
            menu->m_nCursor = slot;
        }
        weapon = (s16) item->m_Weapon;
        if (weapon == player->m_Data.m_CurrentWeapon) {
            menu->m_Hover = slot;
            return;
        }
        player->m_RequestedWeapon = weapon;
        if (*input & 0x80) {
            menu->m_Hover = slot;
            return;
        }
        alternate = gWeaponInfoTable[weapon]->alternate;
        if (alternate != -1 && player->m_Data.m_WeaponList[alternate] != 0) {
            item->m_Weapon = alternate;
        }
    }
#else
    if (slot != -1) {
        entry = (MenuRow *) ((u8 *) menu + slot * sizeof(CBarrelItem));
        if (entry->available == 0) {
            Sound__Play2D(0x1455);
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
        alternate = gWeaponInfoTable[weapon]->alternate;
        if (alternate != -1 && player->m_Data.m_WeaponList[alternate] != 0) {
            entry->weapon.id = alternate;
        }
    }
#endif
    menu->m_Hover = slot;
}
