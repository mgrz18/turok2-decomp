/* Second-module copy of func_00423210 (first module; this one at link address 0x00423130), calling this module's own functions at their link addresses. */
#include "common.h"
#include "structs/menu.h"

s32 func_00253640(void *player, s32 arg1, s32 arg2);

s32 func_0045C130(s32 unused, Menu *event) {
    u8 *player = event->player;

    if (func_00253640(player, 1, *(s32 *)(player + 0x92C)) != 0) {
        event->items[1].flags |= 0x01000000;
    } else {
        event->items[1].flags &= ~0x01000000;
    }
    return 0;
}
