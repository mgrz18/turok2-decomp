#include "common.h"
#include "structs/menu.h"
#include "structs/save.h"

s32 MenuItem_TextLength(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C1404[];

s32 GameNoteMenu_UpdateHealth(MenuItem *item) {
    s32 value = D_800C201C->player.m_Health >> 8;

    sprintf(item->text + (MenuItem_TextLength(item) - 3), D_800C1404, value);
    return 0;
}
