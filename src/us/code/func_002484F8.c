#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

s32 func_0026EFB8(s32 who);

/* A full refill from player who (1-6): kind 0 tops up the ammo of every
 * weapon the player owns, kind 1 the health to 100; returns whether
 * anything was given, and if so notes who gave it. */
s32 func_002484F8(CPlayer *p, s32 kind, s32 who) {
    s32 given = 0;
    s32 slot;
    s32 any;
    s32 i;
    s32 j;
    WeaponInfo *weapon;
    AmmoInfo *info;
    s16 *ammo;

    slot = func_0026EFB8(who);
    if (slot < 1 || slot > 6) {
        return given;
    }
    slot--;
    switch (kind) {
        case 0:
            any = 0;
            for (i = 0; i < 0x23; i++) {
                weapon = D_800AF618[i];
                if (p->m_Data.m_WeaponList[i] == 0) {
                    continue;
                }
                for (j = 0; j < 3; j++) {
                    info = weapon->ammo[j];
                    if (info == NULL) {
                        break;
                    }
                    ammo = &p->m_Data.m_AmmoList[info->type];
                    if (*ammo < info->max) {
                        any = 1;
                    }
                    *ammo = (info->max < *ammo) ? *ammo : info->max;
                }
            }
            given = any;
            if (given) {
                p->m_Data.m_AdonAmmoFlagHelped[slot] = 1;
            }
            break;
        case 1:
            if (p->m_Data.m_Health < 0x6400) {
                p->m_Data.m_Health = 0x6400;
                given = 1;
            }
            if (given) {
                p->m_Data.m_AdonHealthFlagHelped[slot] = kind;
            }
            break;
        default:
            return given;
    }
    return given;
}
