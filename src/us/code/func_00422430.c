#include "common.h"
#include "structs/menu.h"

s32 func_004160F0(void *item);
s32 func_0029E3E0(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C1428[];
extern u16 D_8013094C;

s32 func_00422430(MenuItem *item) {
    s32 value = D_8013094C;
    s32 offset = func_004160F0(item) - 4;

    func_0029E3E0(item->text + offset, D_800C1428, value);
    return 0;
}
