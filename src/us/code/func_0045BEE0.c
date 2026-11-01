/* Second-module copy of func_00422FC0 (first module). The port comes back through a pointer (a stack slot), which gives the ROM's store-then-reload. */
#include "common.h"

void func_00285A68(s32 port);

extern s32 D_800C2020;
extern s32 D_800C2028;
extern s32 D_80130980;
extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_80130998;
extern s32 D_801309AC;
extern s32 D_801309B4;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s8 port;
} MenuController;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ MenuController *controller;
} MenuEvent;

static __inline__ void GetPort(MenuEvent *event, s32 *port) {
    if (D_80130990 != 0) {
        *port = D_800C2028;
    } else {
        *port = event->controller->port;
    }
}

void func_0045BEE0(MenuEvent *event) {
    s32 port;

    D_801309AC = 0;
    D_80130998 = 1;
    D_80130984 = 1;
    D_801309B4 = 0;
    D_80130980 = 0;
    GetPort(event, &port);
    D_800C2020 = 0;
    func_00285A68(port);
}
