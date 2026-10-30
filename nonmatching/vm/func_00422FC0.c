/* NON-MATCHING: 13 of 32 words differ: the ROM reloads the inlined helper parameter from its stack slot after the two branch stores; mine reuses the register */
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

static __inline__ s32 GetPort(MenuEvent *event) {
    if (D_80130990 != 0) {
        return D_800C2028;
    }
    return event->controller->port;
}

/* FAKEMATCH: the ROM keeps this inlined helper's parameter in a stack slot; taking its
 * address reproduces that (see func_00422F14). */
static __inline__ void SelectPort(s32 port) {
    s32 *p = &port;

    D_800C2020 = 0;
    func_00285A68(*p);
}

void func_00422FC0(MenuEvent *event) {
    D_801309AC = 0;
    D_80130998 = 1;
    D_80130984 = 1;
    D_801309B4 = 0;
    D_80130980 = 0;
    SelectPort(GetPort(event));
}
