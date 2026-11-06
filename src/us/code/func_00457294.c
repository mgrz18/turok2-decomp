/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00406858.c, func_00406858, with Turok 2's layouts. */
#include "common.h"
#include "globals/pak.h"

/* Controller pak check for saving: a pad that already holds one of the game's two notes is usable as is
 * (0x7B blocks); otherwise checks the pak's free space against the save size and keeps the largest need. */

extern s32 D_800F70A8;
s32 Pak_GetNoteGameName_2(s32 pad, s32 idx, u8 **out);
void func_0045FBA8(u8 *src, u8 *dst, s32 n);
s32 Str_Compare_2(char *a, char *b);
s32 Pak_GetFreePages_2(s32 pad, s32 *out);
s32 Pak_CountEmptyNotes_2(s32 pad, s32 *out);
s32 Pak_BytesToPages_2(s32 n);

static __inline__ s32 FindNote(s32 pad, char *name) {
    u8 buf[16];
    u8 *ent;
    s32 i;

    for (i = 0; i < 16; i++) {
        Pak_GetNoteGameName_2(pad, i, &ent);
        func_0045FBA8(ent, buf, 16);
        if (Str_Compare_2((char *)buf, name) == 0) {
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

    f1 = FindNote(pad, gPakCharacterNoteName);
    f2 = FindNote(pad, gPakGameNoteName);
    if (f1 != 0 || f2 != 0) {
        *outPad = pad;
        *outBlocks = 0x7B;
        return 1;
    }
    r = Pak_GetFreePages_2(gPakMenuPort, &a);
    if (r == 0) {
        r = Pak_CountEmptyNotes_2(gPakMenuPort, &b);
        if (r == 0) {
            need = Pak_BytesToPages_2(D_800F70A8 + 0x688);
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
