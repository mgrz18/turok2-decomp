#include "common.h"

s32 func_004160F0(void *item);
s32 func_0029E3E0(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C1404[];
extern u8 *D_800C201C;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItem;

s32 func_00421ED4(MenuItem *item) {
    s32 value = *(s32 *)(D_800C201C + 0x1C) >> 8;

    func_0029E3E0(item->text + (func_004160F0(item) - 3), D_800C1404, value);
    return 0;
}
