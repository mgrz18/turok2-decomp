/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00407F6C.c, func_00407F6C, with Turok 2's layouts. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "protos/pak_2.h"

/* Controller pak note list item: prints a note's name (16 characters), extension (4) and size in blocks
 * into the item's text; an empty slot prints blanks. */

/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
void sprintf(char *dst, const char *fmt, ...);

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
    if (gPakMenuLeaving != 0) {
        return 0;
    }
    if (gPakMenuAnyPak != 0) {
        pad = gPakMenuPort;
    } else {
        pad = own->controller->port;
    }
    r = Pak_FindNote_2(pad, idx, &valid, name, ext, &blocks, company, game);
    if (valid != 0 && r == 0) {
        if (Pak_TextHasInvalidChar_2(name, 16) != 0) {
            Str_Copy_2((char *)name, sPakMenuText_OtherGame);
        }
    } else {
        Str_Copy_2((char *)name, sPakMenuText_Empty);
        Str_Copy_2((char *)ext, sPakMenuText_Empty);
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
    sprintf((char *)dst, sPakMenuFmt_3d, blocks);
    return 0;
}
