#include "common.h"
#include "structs/menu.h"

void func_00416644(char *dst, char *fmt, void *arg2, s32 arg3, s32 arg4);

extern char D_0043BC90[];
extern s32 D_800C2028;
extern s32 D_801198A4[2];
extern s32 D_80130940;
extern s32 D_80130980;
extern s32 D_80130990;
extern s32 D_80130998;

static __inline__ void ShowPlayerMessage(u8 *player, char *fmt) {
    func_00416644(*(char **)(player + 0x51C) + 0x53C, fmt, player, *(s32 *)(player + 0xA50), 0);
}

s32 func_004233E8(s32 unused, Menu *event) {
    u8 *player;
    s32 port;
    s32 *p = &port; /* the ROM stores the port through its address and never reads it */

    D_80130980 = 1;
    D_801198A4[0] = 0;
    D_801198A4[1] = 0;
    D_80130940 = 0;
    player = NULL;
    if (D_80130990 != 0) {
        *p = D_800C2028;
    } else {
        player = event->player;
        *p = event->controller->port;
    }
    ShowPlayerMessage(player, D_0043BC90);
    D_80130998 = 0;
    return 1;
}
