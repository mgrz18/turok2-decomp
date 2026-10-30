/* NON-MATCHING: 24 of 36 words differ: the ROM stores the port into an unused stack slot (an inlined helper parameter kept in memory); not reproduced */
#include "common.h"

void func_00416644(char *dst, char *fmt, void *arg2, s32 arg3, s32 arg4);

extern char D_0043BC90[];
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

static __inline__ void ShowPlayerMessage(u8 *player, s32 port, char *fmt) {
    s32 *p = &port;

    func_00416644(*(char **)(player + 0x51C) + 0x53C, fmt, player, *(s32 *)(player + 0xA50), 0);
}

s32 func_004233E8(s32 unused, MenuEvent *event) {
    u8 *player;
    s32 port;

    D_80130980 = 1;
    D_801198A4[0] = 0;
    D_801198A4[1] = 0;
    D_80130940 = 0;
    player = NULL;
    if (D_80130990 != 0) {
        port = D_800C2028;
    } else {
        player = event->player;
        port = event->controller->port;
    }
    ShowPlayerMessage(player, port, D_0043BC90);
    D_80130998 = 0;
    return 1;
}
