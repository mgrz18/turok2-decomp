#include "common.h"
#include "structs/menu.h"

extern char D_800B4B04[];
extern char D_800B4B34[];
extern u8 D_80130954[8]; /* name, padded with spaces */
extern s32 D_801309A8;

s32 CharNoteMenu_UpdateName(MenuItem *item) {
    s32 len = 8;
    u8 *src;
    char *field;
    char *p;
    s32 i;
    char c;

    if (D_801309A8 != 0) {
        item->text = D_800B4B04;
    } else {
        item->text = D_800B4B34;
        src = &D_80130954[7];
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
            *field++ = D_80130954[i];
        }
    }
    return 0;
}
