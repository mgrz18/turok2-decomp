/* The second VM module's copy of src/us/vm1/PakMenu/func_00422430.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 MenuItem_TextLength_2(void *item);
s32 sprintf(char *dst, const char *fmt, ...);

s32 func_0045B350(MenuItem *item) {
    s32 value = gPakProfileRead.kills;
    s32 offset = MenuItem_TextLength_2(item) - 4;

    sprintf(item->text + offset, "%04d", value);
    return 0;
}
