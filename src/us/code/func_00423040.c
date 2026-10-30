#include "common.h"

s32 func_0028591C(s32 port);
s32 func_0041DD90(void *event, s32 port, s32 arg2);

extern s32 D_800C2028;
extern s32 D_80130980;
extern s32 D_80130990;
extern s32 D_801309AC;

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

s32 func_00423040(MenuEvent *event) {
    if (D_80130980 == 0 && D_801309AC == 0) {
        if (GetPort(event) != -1 && func_0028591C(GetPort(event)) != 0) {
            if (func_0041DD90(event, GetPort(event), 0) != 0) {
                D_801309AC = 1;
            }
        }
    }
    return D_801309AC;
}
