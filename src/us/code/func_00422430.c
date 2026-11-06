#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 MenuItem_TextLength(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

s32 CharNoteMenu_UpdateFrags(MenuItem *item) {
    s32 value = gPakProfileRead.kills;
    s32 offset = MenuItem_TextLength(item) - 4;

    sprintf(item->text + offset, "%04d", value);
    return 0;
}
