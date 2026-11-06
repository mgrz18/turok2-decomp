/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00407F6C.c, func_00407F6C, with Turok 2's layouts. */
#include "common.h"
#include "structs/menu.h"

/* Controller pak note list item: prints a note's name (16 characters), extension (4) and size in blocks
 * into the item's text; an empty slot prints blanks. */

extern s32 D_801309BC;
extern s32 D_80130990;
extern s32 D_800C2028; /* pad */
extern char D_800B48D4[];
extern char D_800B48E0[];
extern u8 D_800C1404[];
s32 Pak_FindNote_2(s32 pad, s32 idx, s32 *valid, u8 *name, u8 *ext, s32 *blocks, u8 *company, u8 *game);
s32 Pak_TextHasInvalidChar_2(u8 *p, s32 n);
char *Str_Copy_2(char *dst, char *src);
void sprintf(char *dst, u8 *fmt, s32 a);

s32 PakMenu_NoteText_2(MenuItem *item, Menu *own) {
    u8 name[16];
    u8 ext[8];
    u8 company[8];
    u8 game[8];
    s32 valid;
    s32 blocks;
    s32 pad;
    s32 idx;
    s32 r;
    s32 ok;
    s32 c;
    s32 i;
    u8 *dst;

    idx = item->index - 3;
    c = 0x20;
    if (D_801309BC != 0) {
        return 0;
    }
    if (D_80130990 != 0) {
        pad = D_800C2028;
    } else {
        pad = own->controller->port;
    }
    r = Pak_FindNote_2(pad, idx, &valid, name, ext, &blocks, company, game);
    if (valid != 0 && r == 0) {
        if (Pak_TextHasInvalidChar_2(name, 16) != 0) {
            Str_Copy_2((char *)name, D_800B48D4);
        }
    } else {
        Str_Copy_2((char *)name, D_800B48E0);
        Str_Copy_2((char *)ext, D_800B48E0);
        blocks = 0;
    }
    dst = (u8 *)item->text;
    ok = 1;
    dst += 3;
    for (i = 0; i < 16; i++) {
        if (ok != 0) {
            c = name[i];
        }
        if (c != 0) {
            *dst = c;
        } else {
            ok = 0;
            c = 0x20;
            *dst = c;
        }
        dst++;
    }
    ok = 1;
    dst += ok;
    for (i = 0; i < 4; i++) {
        if (ok != 0) {
            c = ext[i];
        }
        if (c != 0) {
            *dst = c;
        } else {
            ok = 0;
            c = 0x20;
            *dst = c;
        }
        dst++;
    }
    *dst = *dst + 2;
    sprintf((char *)dst, D_800C1404, blocks);
    return 0;
}
