#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

extern s32 D_80110058;
extern s32 D_8011005C;

void func_0024BF1C(CBarrel *menu, s32 rows, s32 *owned, s32 perRow, CPlayer *player);

/* Opens the weapon menu: flags the weapons the player can pick (an owned
 * weapon usable in the current mode, unless it is the lower of a pair the
 * player owns both of), lays them out on one or two rings, then steps the
 * cursor by CREDITS.md	   Makefile	   docs		scripts  turok2.us.toml
Dockerfile	   README.md	   include	src	 us
Dockerfile.native  baserom.us.z64  nonmatching	test	 versions
LICENSE		   build	   references	tools until it rests on the queued weapon. */
void func_0024C0B4(CBarrel *menu, s32 dir, CPlayer *player) {
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
        if (player->m_Data.m_WeaponList[i] == 0) {
            ok = 0;
        } else if (!((player->m_WeaponActor.m_AI.m_dwFlags & 0x80) ? (info->flags & 2) : (info->flags & 1))) {
            ok = 0;
        } else {
            alt = info->alternate;
            ok = 1;
            if (player->m_Data.m_WeaponList[alt] != 0) {
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
    menu->m_Turn = 45.0f;
    if (menu->m_State == 0 || menu->m_State == 3) {
        menu->m_State = 1;
    }
    menu->m_Step = -dir;
    menu->m_Glow = 0;
    while (menu->m_Items[menu->m_nCursor].m_Weapon != player->m_RequestedWeapon) {
        menu->m_nCursor += dir;
        if (menu->m_nCursor < 0) {
            menu->m_nCursor = menu->m_nCount - 1;
        } else if (menu->m_nCursor >= menu->m_nCount) {
            menu->m_nCursor = 0;
        }
    }
}
