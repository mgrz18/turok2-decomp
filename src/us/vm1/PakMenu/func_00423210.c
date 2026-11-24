#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"
#include "structs/camera.h"

s32 CPlayer__CanGetRefill(struct CPlayer *player, s32 kind, s32 arg); /* code/CPlayer's definition */

s32 SavePortalMenu_UpdateHealth(s32 unused, Menu *event) {
    u8 *player = event->player;

    if (CPlayer__CanGetRefill((struct CPlayer *) player, 1, ((CPlayer *)player)->m_Data.m_CheckPointWarpID) != 0) {
        event->items[1].flags |= 0x01000000;
    } else {
        event->items[1].flags &= ~0x01000000;
    }
    return 0;
}
