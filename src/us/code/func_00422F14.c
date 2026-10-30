#include "common.h"

void func_00285A68(s32 port);
void func_00426480(s32 port);

extern s32 D_800C2020;
extern s32 D_800C2028;
extern s32 D_80130980;
extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_80130994;
extern s32 D_80130998;
extern s32 D_801309A0;
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

static __inline__ s32 GetPort(MenuEvent *event) {
    if (D_80130990 != 0) {
        return D_800C2028;
    }
    return event->controller->port;
}

/* FAKEMATCH: the ROM keeps this inlined helper's parameter in a stack slot (stored, the
 * call still reads the register); taking its address reproduces that. The original helper
 * probably took the address for another purpose. */
static __inline__ void SelectPort(s32 port) {
    s32 *p = &port;

    func_00285A68(*p);
}

void func_00422F14(MenuEvent *event) {
    MenuController *controller = event->controller;

    D_80130990 = 0;
    D_800C2020 = 0;
    D_801309AC = 0;
    D_80130998 = 1;
    D_80130984 = 1;
    D_801309A0 = 1;
    D_80130994 = 0;
    D_801309B4 = 0;
    D_80130980 = 0;
    SelectPort(controller->port);
    func_00426480(GetPort(event));
}
