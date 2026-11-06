/* Second-module copy of CharNoteMenu_UpdateName (first module): the name, right-aligned in an 8-space field at text+11. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern char D_800B4B04[];
extern char D_800B4B34[];
extern s32 D_801309A8;

s32 CharNoteMenu_UpdateName_2(MenuItem *item) {
    s32 len = 8;
    u8 *src;
    char *field;
    char *dst;
    s32 i;
    char *p;
    s32 space;

    if (D_801309A8 != 0) {
        item->text = D_800B4B04;
    } else {
        item->text = D_800B4B34;
        src = &gPakProfileRead.name[7];
        while (*src-- == ' ' && len != 0) {
            len--;
        }
        field = item->text;
        space = ' ';
        field += 11;
        p = field + 7;
        for (i = 7; i >= 0; i--) {
            *p = space;
            p--;
        }
        p = field + 8;
        dst = p - len;
        for (i = 0; i < len; i++) {
            *dst++ = gPakProfileRead.name[i];
        }
    }
    return 0;
}
