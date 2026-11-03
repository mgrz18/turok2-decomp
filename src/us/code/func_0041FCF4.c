/* First-module twin of src/us/code/func_00458C14.c. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00408118.c, func_00408118, with Turok 2's layouts. */
#include "common.h"

/* Controller pak menu item: picks the item's text for the pak state and prints the pak's free blocks and
 * the blocks a save needs into it ("%3d"-style, three characters each). */

typedef struct {
    char c[4];
} Chars4;

typedef struct {
    u8 pad0[4];
    s8 pad; /* 0x04 */
} MenuPad;

typedef struct {
    u8 pad0[0x20];
    MenuPad *pad; /* 0x20 */
} MenuOwner;

typedef struct {
    u8 pad0[0x14];
    char *text; /* 0x14 */
} MenuItem;

extern s32 D_80130990;
extern s32 D_800C2028; /* pad */
extern s32 D_80130984;
extern s32 D_80130994;
extern s32 D_801309B4;
extern s32 D_800F70A8;
extern Chars4 D_800C1408;
extern u8 D_800C1404[];
extern char D_800B48B4[];
extern char D_800B4914[];
extern char D_800C140C[];
s32 func_004267A0(s32 pad, s32 *out);
u32 func_00426E6C(u32 n);
void func_0029E3E0(char *dst, u8 *fmt, s32 a);

s32 func_0041FCF4(MenuItem *item, MenuOwner *own) {
    Chars4 buf;
    char *bp;
    s32 pad;
    s32 freeb;
    s32 need;
    char *dst;
    s32 r;
    char *t;
    u8 *fmt;

    if (D_80130990 != 0) {
        pad = D_800C2028;
    } else {
        pad = own->pad->pad;
    }
    buf = D_800C1408;
    bp = buf.c;
    if (D_80130984 != 0) {
        item->text = D_800B48B4;
        if (func_004267A0(pad, &freeb) == 0) {
            if (D_801309B4 != 0) {
                need = func_00426E6C(0x18);
            } else {
                need = func_00426E6C(D_800F70A8 + 0x688);
            }
            dst = item->text;
            dst += 0xC;
            func_0029E3E0(buf.c, D_800C1404, freeb);
            *dst = buf.c[0];
            dst++;
            *dst = buf.c[1];
            dst++;
            *dst = buf.c[2];
            func_0029E3E0(buf.c, D_800C1404, need);
            dst += 0xC;
            *dst = buf.c[0];
            dst++;
            *dst = buf.c[1];
            dst++;
            *dst = buf.c[2];
        }
    } else {
        if (D_80130990 != 0) {
            t = D_800B4914;
        } else if (D_80130994 != 0) {
            t = D_800B48B4;
        } else {
            goto none;
        }
        item->text = t;
        r = func_004267A0(pad, &freeb);
        need = func_00426E6C(D_800F70A8 + 0x688);
        dst = item->text;
        if (r == 0) {
            dst += 0xC;
            fmt = D_800C1404;
            func_0029E3E0(bp, fmt, freeb);
            *dst = buf.c[0];
            dst++;
            *dst = buf.c[1];
            dst++;
            *dst = buf.c[2];
            func_0029E3E0(bp, D_800C1404, need);
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
