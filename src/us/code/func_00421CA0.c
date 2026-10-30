#include "common.h"

s32 func_004160F0(void *item);
s32 func_0029E3E0(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C140C[];
extern char D_800C1410[];
extern char D_800B45E0[];
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

s32 func_00421CA0(MenuItem *item, MenuEvent *event) {
    s32 port;

    if (D_80130990 != 0) {
        item->text = D_800C140C;
    } else {
        port = event->controller->port;
        item->text = D_800B45E0;
        func_0029E3E0(item->text + (func_004160F0(item) - 1), D_800C1410, port + 1);
    }
    return 0;
}
