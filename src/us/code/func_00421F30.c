#include "common.h"
#include "structs/menu.h"
#include "structs/save.h"

s32 MenuItem_TextLength(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

s32 GameNoteMenu_UpdateLives(MenuItem *item) {
    s32 value = gLoadBuffer->player.m_Lives;

    sprintf(item->text + (MenuItem_TextLength(item) - 2), "%2d", value);
    return 0;
}
