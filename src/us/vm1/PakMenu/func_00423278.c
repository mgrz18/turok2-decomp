#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"
#include "structs/camera.h"

s32 Cinema_IsActive(void);
s32 Cinema_IsFinished(void);
s32 CPlayer__RefillFrom(u8 *player, s32 arg1, s32 arg2);
void Cinema_StopAtEnd(void);
void Cinema_RequestById(s32 sound);

static __inline__ s32 func_00422EE0_inline(void) {
    if (Cinema_IsActive() != 0 && Cinema_IsFinished() == 0) {
        return 1;
    }
    return 0;
}

s32 SavePortalMenu_SelectHealth(s32 unused, Menu *event) {
    u8 *player = event->player;
    s32 ok;

    if (func_00422EE0_inline()) {
        return 0;
    }
    ok = CPlayer__RefillFrom(player, 1, ((CPlayer *)player)->m_Data.m_CheckPointWarpID);
    event->cursor = 0;
    if (ok) {
        Cinema_StopAtEnd();
        Cinema_RequestById(0x192);
    }
    return 0;
}
