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

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s16 ammoType;
} PickupInfo;

extern PickupInfo *func_0023EF64(s32);

/* Whether the player has room for more of a pickup ammo, counting only the
 * weapons held. */
s32 func_00253C14(CPlayer *player, s32 pickup) {
    PickupInfo *info = func_0023EF64(pickup);
    s32 type;
    s16 *ammo;

    if (info == NULL) {
        return 0;
    }
    type = info->ammoType;
    ammo = &player->m_Data.m_AmmoList[type];
    return *ammo < GetMaxAmmo(player, type, 1);
}
