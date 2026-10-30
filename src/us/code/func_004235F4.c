#include "common.h"

s32 func_004160F0(void *item);
s32 func_0029E3E0(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800B50B4[];
extern char D_800B50E4[];
extern char D_800C1430[];
extern s32 D_800C2028;
extern s32 D_80130964;
extern s32 D_80130990;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItem;

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

s32 func_004235F4(MenuItem *item, MenuEvent *event) {
    s32 port = GetPort(event);

    if (D_80130964 != 0) {
        item->text = D_800B50B4;
    } else {
        item->text = D_800B50E4;
        func_0029E3E0(item->text + (func_004160F0(item) - 2), D_800C1430, port + 1);
    }
    return 0;
}
