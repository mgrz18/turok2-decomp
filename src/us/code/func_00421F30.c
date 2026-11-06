#include "common.h"
#include "structs/menu.h"
#include "structs/save.h"

s32 MenuItem_TextLength(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C1414[];

s32 GameNoteMenu_UpdateLives(MenuItem *item) {
    s32 value = D_800C201C->player.m_Lives;

    sprintf(item->text + (MenuItem_TextLength(item) - 2), D_800C1414, value);
    return 0;
}
