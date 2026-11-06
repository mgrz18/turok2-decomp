#include "common.h"
#include "structs/menu.h"

s32 CPlayer__CanGetRefill(void *player, s32 arg1, s32 arg2);

s32 SavePortalMenu_UpdateHealth(s32 unused, Menu *event) {
    u8 *player = event->player;

    if (CPlayer__CanGetRefill(player, 1, *(s32 *)(player + 0x92C)) != 0) {
        event->items[1].flags |= 0x01000000;
    } else {
        event->items[1].flags &= ~0x01000000;
    }
    return 0;
}
