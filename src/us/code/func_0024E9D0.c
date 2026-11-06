#include "common.h"
#include "structs/region.h"
#include "structs/player.h"
#include "globals/game.h"


CRegionInfo *GameObject_GetRegionInfo(void *actor);
s32 CPlayer__SetState(CPlayer *player, void *actor, s32 state);

static __inline__ s32 func_0024E9D0_isSpecial(CPlayer *player) {
    if (player->m_Mode == 18 || player->m_Mode == 16 || player->m_Mode == 17) {
        return 1;
    }
    return 0;
}

static __inline__ s32 func_0024E9D0_hasKey(CPlayer *player, s32 i) {
    if (gEngineApp_m_Settings.cheatFlags & 0x10) {
        return 1;
    }
    return player->m_Data.m_SpecialPickupCount[382 + i] & 0xF;   /* the two key pickups of the region */
}

/* Enters state 32 when the actor stands in a region flagged 0x1000 whose
 * key requirements (0x800 on the flags, 0x10 on flags54) the player meets.
 * Returns whether it did. */
s32 CPlayer__CheckJumpPad(CPlayer *player, void *actor) {
    CRegionInfo *region;

    if (player->m_TimeOffGround > 0.0f) {
        return 0;
    }
    if (func_0024E9D0_isSpecial(player)) {
        return 0;
    }
    region = GameObject_GetRegionInfo(actor);
    if (region != NULL && (region->m_wFlags52 & 0x1000)) {
        if (!(region->m_dwFlags & 0x800)) {
            if (region->m_wFlags54 & 0x10) {
                goto second;
            }
            goto enter;
        } else {
            if (func_0024E9D0_hasKey(player, 0)) {
                goto enter;
            }
            if (!(region->m_wFlags54 & 0x10)) {
                return 0;
            }
        }
    second:
        if (func_0024E9D0_hasKey(player, 1)) {
        enter:
            CPlayer__SetState(player, actor, 32);
            return 1;
        }
    }
    return 0;
}
