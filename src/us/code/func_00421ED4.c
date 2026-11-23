#include "common.h"
#include "structs/menu.h"
#include "structs/save.h"

s32 MenuItem_TextLength(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

extern char sPakMenuFmt_3d[];

s32 GameNoteMenu_UpdateHealth(MenuItem *item) {
    s32 value = gLoadBuffer->player.m_Health >> 8;

    sprintf(item->text + (MenuItem_TextLength(item) - 3), sPakMenuFmt_3d, value);
    return 0;
}
