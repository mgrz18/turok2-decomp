/* The second VM module's copy of src/us/vm1/PakMenu/func_00421ED4.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "structs/save.h"

s32 MenuItem_TextLength_2(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */


s32 func_0045ADF4(MenuItem *item) {
    s32 value = gLoadBuffer->player.m_Health >> 8;

    sprintf(item->text + (MenuItem_TextLength_2(item) - 3), sPakMenuFmt_3d, value);
    return 0;
}
