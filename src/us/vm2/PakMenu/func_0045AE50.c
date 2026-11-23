/* The second VM module's copy of src/us/vm1/PakMenu/func_00421F30.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/menu.h"
#include "structs/save.h"

s32 MenuItem_TextLength_2(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

s32 func_0045AE50(MenuItem *item) {
    s32 value = gLoadBuffer->player.m_Lives;

    sprintf(item->text + (MenuItem_TextLength_2(item) - 2), "%2d", value);
    return 0;
}
