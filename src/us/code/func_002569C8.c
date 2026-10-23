#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 max;
} AmmoDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ AmmoDef *ammo[3];
} WeaponDef;

extern WeaponDef *D_800AF618[];

/* Fills the ammo of every held weapon to its maximum; returns whether any
 * ammo was added. */
s32 func_002569C8(CPlayer *player) {
    s32 added = 0;
    s32 w;
    s32 i;
    WeaponDef *def;
    AmmoDef *ammo;
    s16 *count;
    s16 n;

    for (w = 0; w < 35; w++) {
        def = D_800AF618[w];
        if ((s8)player->m_Data.m_WeaponList[w] == 0) {
            continue;
        }
        for (i = 0; i < 3; i++) {
            ammo = def->ammo[i];
            if (ammo == NULL) {
                break;
            }
            count = &player->m_Data.m_AmmoList[ammo->type];
            if (*count < ammo->max) {
                added = 1;
            }
            n = ammo->max;
            if (ammo->max < *count) {
                n = *count;
            }
            *count = n;
        }
    }
    return added;
}
