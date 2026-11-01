/* Second-module copy of func_004233E8 (first module). The port comes back through a pointer (a stack slot the ROM stores but never reads). */
#include "common.h"

void func_00416644(char *dst, char *fmt, void *player, s32 controller, s32 arg4);

extern char D_0043BBB0[];
extern s32 D_800C2028;
extern s32 D_801198A4[2];
extern s32 D_80130940;
extern s32 D_80130980;
extern s32 D_80130990;
extern s32 D_80130998;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s8 port;
} MenuController;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ u8 *player;
    /* 0x20 */ MenuController *controller;
} MenuEvent;

static __inline__ u8 *GetPlayer(MenuEvent *event, s32 *port) {
    u8 *player = NULL;

    if (D_80130990 != 0) {
        *port = D_800C2028;
    } else {
        player = event->player;
        *port = event->controller->port;
    }
    return player;
}

s32 func_0045C308(s32 unused, MenuEvent *event) {
    u8 *player;
    s32 port;

    D_80130980 = 1;
    D_801198A4[0] = 0;
    D_801198A4[1] = 0;
    D_80130940 = 0;
    player = GetPlayer(event, &port);
    func_00416644(*(char **)(player + 0x51C) + 0x53C, D_0043BBB0, player, *(s32 *)(player + 0xA50), 0);
    D_80130998 = 0;
    return 1;
}
