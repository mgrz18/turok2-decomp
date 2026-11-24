/* First-module twin of src/us/code/PakMenu_FreePagesText_2.c. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00408118.c, func_00408118, with Turok 2's layouts. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "globals/scene.h"
#include "structs/twin_pakmenu.h"
#include "protos/pak.h"

/* Controller pak menu item: picks the item's text for the pak state and prints the pak's free blocks and
 * the blocks a save needs into it ("%3d"-style, three characters each). */


extern Chars4 sPakMenuText_Spaces3; /* local: menu_text.h is not included here: sPakMenuText_Spaces3 is copied as a Chars4 (one lw/sw) */
extern u8 sPakMenuFmt_3d[]; /* local: menu_text.h is not included here: sPakMenuText_Spaces3 is copied as a Chars4 (one lw/sw) */
extern char sPakMenuText_FreePagesXxxNeededXxx_B48B4[]; /* local: menu_text.h is not included here: sPakMenuText_Spaces3 is copied as a Chars4 (one lw/sw) */
extern char sPakMenuText_FreePagesXxxNeededXxx_B4914[]; /* local: menu_text.h is not included here: sPakMenuText_Spaces3 is copied as a Chars4 (one lw/sw) */
extern char sPakMenuText_Space[]; /* local: menu_text.h is not included here (the Chars4 view above) */
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
void sprintf(char *dst, const char *fmt, ...);

s32 PakMenu_FreePagesText(MenuItem *item, Menu *own) {
    Chars4 buf;
    char *bp;
    s32 pad;
    s32 freeb;
    s32 need;
    char *dst;
    s32 r;
    char *t;
    u8 *fmt;

    if (gPakMenuAnyPak != 0) {
        pad = gPakMenuPort;
    } else {
        pad = own->controller->port;
    }
    buf = sPakMenuText_Spaces3;
    bp = buf.c;
    if (gPakMenuIsSave != 0) {
        item->text = sPakMenuText_FreePagesXxxNeededXxx_B48B4;
        if (Pak_GetFreePages(pad, &freeb) == 0) {
            if (gPakMenuIsProfile != 0) {
                need = Pak_BytesToPages(0x18);
            } else {
                need = Pak_BytesToPages(gScene.unk_30 + 0x688);
            }
            dst = item->text;
            dst += 0xC;
            sprintf(buf.c, sPakMenuFmt_3d, freeb);
            *dst = buf.c[0];
            dst++;
            *dst = buf.c[1];
            dst++;
            *dst = buf.c[2];
            sprintf(buf.c, sPakMenuFmt_3d, need);
            dst += 0xC;
            *dst = buf.c[0];
            dst++;
            *dst = buf.c[1];
            dst++;
            *dst = buf.c[2];
        }
    } else {
        if (gPakMenuAnyPak != 0) {
            t = sPakMenuText_FreePagesXxxNeededXxx_B4914;
        } else if (gPakMenuIsLoad != 0) {
            t = sPakMenuText_FreePagesXxxNeededXxx_B48B4;
        } else {
            goto none;
        }
        item->text = t;
        r = Pak_GetFreePages(pad, &freeb);
        need = Pak_BytesToPages(gScene.unk_30 + 0x688);
        dst = item->text;
        if (r == 0) {
            dst += 0xC;
            fmt = sPakMenuFmt_3d;
            sprintf(bp, fmt, freeb);
            *dst = buf.c[0];
            dst++;
            *dst = buf.c[1];
            dst++;
            *dst = buf.c[2];
            sprintf(bp, sPakMenuFmt_3d, need);
            dst += 0xC;
            *dst = buf.c[0];
            dst++;
            *dst = buf.c[1];
            dst++;
            *dst = buf.c[2];
        }
    }
    goto done;
none:
    item->text = sPakMenuText_Space;
done:
    return 0;
}
