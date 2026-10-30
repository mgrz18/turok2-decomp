#include "common.h"

s32 func_004160F0(void *item);
s32 func_0029E3E0(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C1410[];
extern s32 D_800C2028;
extern char D_800B47F4[];
extern char D_800B4804[];
extern char D_800B48F8[];
extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_801309B4;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItem;

s32 func_00421BE4(MenuItem *item) {
    s32 player;

    if (D_801309B4 != 0 && D_80130984 != 0) {
        item->text = D_800B47F4;
    } else if (D_801309B4 == 0 && D_80130984 != 0) {
        item->text = D_800B4804;
    } else if (D_80130990 != 0) {
        player = D_800C2028;
        item->text = D_800B48F8;
        func_0029E3E0(item->text + (func_004160F0(item) - 1), D_800C1410, player + 1);
    }
    return 0;
}
