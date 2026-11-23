/* Second-module copy of CharNoteMenu_UpdateName (first module): the name, right-aligned in an 8-space field at text+11. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"


s32 CharNoteMenu_UpdateName_2(MenuItem *item) {
    s32 len = 8;
    u8 *src;
    char *field;
    char *dst;
    s32 i;
    char *p;
    s32 space;

    if (gCharNoteMenuSaving != 0) {
        item->text = sCharNoteMenuText_NameExt;
    } else {
        item->text = sCharNoteMenuText_Name;
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
