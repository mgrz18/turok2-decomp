#include "common.h"

s32 func_004160F0(void *item);
s32 func_0029E3E0(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C1414[];
extern u8 *D_800C201C;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItem;

s32 func_00421F30(MenuItem *item) {
    s32 value = *(s16 *)(D_800C201C + 0x22);

    func_0029E3E0(item->text + (func_004160F0(item) - 2), D_800C1414, value);
    return 0;
}
