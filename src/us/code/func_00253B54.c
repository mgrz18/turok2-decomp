#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 max;
} AmmoDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ AmmoDef *ammo[3]; /* [1] is the lower ammo */
} WeaponDef;

extern WeaponDef *D_800AF618[];

/* func_00253AC0, inlined. */
static __inline__ s32 GetMaxAmmo(CPlayer *player, s32 type, s32 heldOnly) {
    s32 max;
    s32 w;
    s32 i;
    WeaponDef *def;
    AmmoDef *ammo;
    s32 n;

    if (type == -1) {
        return 0;
    }
    max = 0;
    for (w = 0; w < 35; w++) {
        if (heldOnly && (s8)player->m_Data.m_WeaponList[w] == 0) {
            continue;
        }
        def = D_800AF618[w];
        for (i = 0; i < 3; i++) {
            ammo = def->ammo[i];
            if (ammo == NULL) {
                break;
            }
            if (ammo->type == type) {
                n = ammo->max;
                if (n < max) {
                    n = max;
                }
                max = n;
            }
        }
    }
    return max;
}

/* Adds rounds of an ammo type up to the most any weapon carries; returns
 * whether there was room. */
/* Adds rounds of an ammo type, up to the most any weapon carries; returns
 * whether there was room. */
s32 func_00253B54(CPlayer *player, s32 type, s32 count) {
    s16 *ammo = &player->m_Data.m_AmmoList[type];
    s32 max = GetMaxAmmo(player, type, 0);
    s32 room = max > *ammo;

    if (room) {
        s32 n;

        *ammo += count;
        n = *ammo;
        if (max < n) {
            n = max;
        }
        *ammo = n;
    }
    return room;
}
