/* First-module twin of src/us/code/PakMenu_FreePagesText_2.c. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00408118.c, func_00408118, with Turok 2's layouts. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

/* Controller pak menu item: picks the item's text for the pak state and prints the pak's free blocks and
 * the blocks a save needs into it ("%3d"-style, three characters each). */

typedef struct {
    char c[4];
} Chars4;

extern s32 D_800F70A8;
extern Chars4 D_800C1408;
extern u8 D_800C1404[];
extern char D_800B48B4[];
extern char D_800B4914[];
extern char D_800C140C[];
s32 Pak_GetFreePages(s32 pad, s32 *out);
u32 Pak_BytesToPages(uptr n);
#ifdef TARGET_PORT
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
void sprintf(char *dst, const char *fmt, ...);
#else
void sprintf(char *dst, u8 *fmt, s32 a);
#endif

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
    buf = D_800C1408;
    bp = buf.c;
    if (gPakMenuIsSave != 0) {
        item->text = D_800B48B4;
        if (Pak_GetFreePages(pad, &freeb) == 0) {
            if (gPakMenuIsProfile != 0) {
                need = Pak_BytesToPages(0x18);
            } else {
                need = Pak_BytesToPages(D_800F70A8 + 0x688);
            }
            dst = item->text;
            dst += 0xC;
            sprintf(buf.c, D_800C1404, freeb);
            *dst = buf.c[0];
            dst++;
            *dst = buf.c[1];
            dst++;
            *dst = buf.c[2];
            sprintf(buf.c, D_800C1404, need);
            dst += 0xC;
            *dst = buf.c[0];
            dst++;
            *dst = buf.c[1];
            dst++;
            *dst = buf.c[2];
        }
    } else {
        if (gPakMenuAnyPak != 0) {
            t = D_800B4914;
        } else if (gPakMenuIsLoad != 0) {
            t = D_800B48B4;
        } else {
            goto none;
        }
        item->text = t;
        r = Pak_GetFreePages(pad, &freeb);
        need = Pak_BytesToPages(D_800F70A8 + 0x688);
        dst = item->text;
        if (r == 0) {
            dst += 0xC;
            fmt = D_800C1404;
            sprintf(bp, fmt, freeb);
            *dst = buf.c[0];
            dst++;
            *dst = buf.c[1];
            dst++;
            *dst = buf.c[2];
            sprintf(bp, D_800C1404, need);
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
    item->text = D_800C140C;
done:
    return 0;
}
