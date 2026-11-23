/* The second VM module's copy of src/us/vm1/PakMenu/func_00422060.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/menu.h"
#include "structs/save.h"

s32 MenuItem_TextLength_2(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

s32 func_0045AF80(MenuItem *item) {
    s32 seconds = gLoadBuffer->seconds;
    s32 minutes = gLoadBuffer->minutes;
    s32 hours = gLoadBuffer->hours;

    sprintf(item->text + (MenuItem_TextLength_2(item) - 9), "%3d.%02d.%02d", hours, minutes, seconds);
    return 1;
}
