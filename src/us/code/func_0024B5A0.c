#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

extern f32 D_800B6D28;      /* frame step */

s32 func_0024E700(CPlayer *player, s32 weapon);
s32 func_002548FC(CPlayer *player);
s32 func_00254F88(CBarrel *menu, CPlayer *player);
void func_0024C47C(CBarrel *menu, CPlayer *player);
void func_00210EF0(f32 *angle);

/* Runs the weapon menu: marks the weapons the player can use, steps the
 * cursor with the next/previous weapon buttons and turns the wheel towards
 * the selected slot. */
void func_0024B5A0(CBarrel *menu, CPlayer *player) {
    s32 step;
    s32 i;
    s32 dir;
    f32 from;
    f32 to;
    f32 diff;
    f32 turn;

    dir = 0;
    if (menu->m_State == 1 || menu->m_State == 2) {
        for (i = 0; i < menu->m_nCount; i++) {
            if (func_0024E700(player, menu->m_Items[i].m_Weapon) != 0) {
                menu->m_Items[i].m_bAvailable = 1;
            } else {
                menu->m_Items[i].m_bAvailable = 0;
            }
        }
    }
    if (func_002548FC(player) != 0) {
        dir = 0;
    } else if (player->m_ContState.m_dwButtonsSingle & 0x4000) {
        dir = 1;
    } else if (player->m_ContState.m_dwButtonsSingle & 0x8000) {
        dir = -1;
    }
    if ((menu->m_State == 0 && dir != 0)
        || (func_00254F88(menu, player) != 0 && menu->m_State != 0)
        || (menu->m_nCursor != -1 && menu->m_Items[menu->m_nCursor].m_Weapon != player->m_RequestedWeapon)) {
        func_0024C47C(menu, player);
    }
    if (dir != 0) {
        do {
            menu->m_nCursor += dir;
            if (menu->m_nCursor >= menu->m_nCount) {
                menu->m_nCursor = 0;
            } else if (menu->m_nCursor < 0) {
                menu->m_nCursor = menu->m_nCount - 1;
            }
        } while (menu->m_Items[menu->m_nCursor].m_bAvailable == 0);
        player->m_RequestedWeapon = menu->m_Items[menu->m_nCursor].m_Weapon;
        menu->m_Step = -dir;
        menu->m_Turn = 45.0f;
        if (menu->m_State == 0 || menu->m_State == 3) {
            menu->m_State = 1;
        }
    }
    if (menu->m_State != 0) {
        from = menu->m_Spin;
        to = menu->m_Items[menu->m_nCursor].m_Angle;
        step = menu->m_Step;
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
        menu->m_Spin += turn;
    }
}
