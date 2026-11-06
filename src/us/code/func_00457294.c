/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00406858.c, func_00406858, with Turok 2's layouts. */
#include "common.h"

/* Controller pak check for saving: a pad that already holds one of the game's two notes is usable as is
 * (0x7B blocks); otherwise checks the pak's free space against the save size and keeps the largest need. */

extern char D_800B45C0[];
extern char D_800B45CC[];
extern s32 D_800C2028; /* pad */
extern s32 D_800F70A8;
s32 func_00426898(s32 pad, s32 idx, u8 **out);
void func_00426BA8(u8 *src, u8 *dst, s32 n);
s32 func_00426C74(char *a, char *b);
s32 func_004266C0(s32 pad, s32 *out);
s32 func_0042662C(s32 pad, s32 *out);
s32 Str_Length(s32 n);

static __inline__ s32 FindNote(s32 pad, char *name) {
    u8 buf[16];
    u8 *ent;
    s32 i;

    for (i = 0; i < 16; i++) {
        func_00426898(pad, i, &ent);
        func_00426BA8(ent, buf, 16);
        if (func_00426C74((char *)buf, name) == 0) {
            return 1;
        }
    }
    return 0;
}

s32 PakMenu_CheckSlotForSave_2(s32 unused, s32 pad, s32 *outPad, s32 *outBlocks) {
    s32 f1;
    s32 f2;
    s32 r;
    s32 a;
    s32 b;
    s32 need;

    f1 = FindNote(pad, D_800B45C0);
    f2 = FindNote(pad, D_800B45CC);
    if (f1 != 0 || f2 != 0) {
        *outPad = pad;
        *outBlocks = 0x7B;
        return 1;
    }
    r = func_004266C0(D_800C2028, &a);
    if (r == 0) {
        r = func_0042662C(D_800C2028, &b);
        if (r == 0) {
            need = Str_Length(D_800F70A8 + 0x688);
            if (b == 0 || a < need) {
                if (*outBlocks < need) {
                    *outBlocks = need;
                    *outPad = pad;
                }
                return 0;
            }
        }
    }
    return r == 0;
}
