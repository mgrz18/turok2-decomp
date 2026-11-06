#include "common.h"
#include "structs/menu.h"

s32 MenuItem_TextLength(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C1428[];
extern u16 gPakProfileRead_kills;

s32 CharNoteMenu_UpdateFrags(MenuItem *item) {
    s32 value = gPakProfileRead_kills;
    s32 offset = MenuItem_TextLength(item) - 4;

    sprintf(item->text + offset, D_800C1428, value);
    return 0;
}
