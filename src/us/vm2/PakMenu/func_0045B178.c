#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"


s32 CharNoteMenu_UpdateName_2(MenuItem *item) {
    s32 len = 8;
    u8 *src;
    char *field;
    char *p;
    s32 i;
    char c;

    if (gCharNoteMenuSaving != 0) {
        item->text = sCharNoteMenuText_NameExt;
    } else {
        item->text = sCharNoteMenuText_Name;
        src = &gPakProfileRead.name[7];
        while (*src-- == ' ' && len != 0) {
            len--;
        }
        field = item->text;
        field += 11;
        c = ' ';
        p = &field[7];
        for (i = 7; i >= 0; i--) {
            *p-- = c;
        }
        field = &field[8 - len];
        for (i = 0; i < len; i++) {
            *field++ = gPakProfileRead.name[i];
        }
    }
    return 0;
}
