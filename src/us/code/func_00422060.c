#include "common.h"
#include "structs/menu.h"
#include "structs/save.h"

s32 MenuItem_TextLength(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C1418[];

s32 GameNoteMenu_UpdateTime(MenuItem *item) {
    s32 seconds = gLoadBuffer->seconds;
    s32 minutes = gLoadBuffer->minutes;
    s32 hours = gLoadBuffer->hours;

    sprintf(item->text + (MenuItem_TextLength(item) - 9), D_800C1418, hours, minutes, seconds);
    return 1;
}
