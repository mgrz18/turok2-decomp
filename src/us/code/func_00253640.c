#include "common.h"
#include "structs/player.h"

extern s32 Level_GetHub(s32);

/* Whether Adon has not yet helped with ammo (kind 0) or health (kind 1) on
 * the hub level that Level_GetHub maps arg to (1-6). */
s32 CPlayer__CanGetRefill(CPlayer *player, s32 kind, s32 arg) {
    u32 hub = Level_GetHub(arg) - 1;

    if (hub >= 6) {
        return 0;
    }
    switch (kind) {
        case 0:
        default:
            return player->m_Data.m_AdonAmmoFlagHelped[hub] == 0;
        case 1:
            return player->m_Data.m_AdonHealthFlagHelped[hub] == 0;
    }
}
