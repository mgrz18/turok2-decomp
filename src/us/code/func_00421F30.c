#include "common.h"
#include "structs/menu.h"
#include "structs/save.h"

s32 func_004160F0(void *item);
s32 func_0029E3E0(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C1414[];

s32 func_00421F30(MenuItem *item) {
    s32 value = D_800C201C->player.m_Lives;

    func_0029E3E0(item->text + (func_004160F0(item) - 2), D_800C1414, value);
    return 0;
}
