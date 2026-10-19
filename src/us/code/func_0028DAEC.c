/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0023BC64.c, func_0023BC64.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

extern B09SlotA D_80121318[];
extern B09LNode D_80120FC0[];
extern B09SlotCtl D_80121390;
s32 func_0028DAEC(void) {
    s16 idx = D_80121390.cur++;
    if ((s16)D_80121390.cur >= 0x18) {
        D_80121390.cur = D_80121390.start;
    }
    if (D_80121318[idx].a != 0xFF) {
        D_80120FC0[D_80121318[idx].a].tlb = 0xFF;
        D_80121318[idx].a = 0xFF;
    }
    if (D_80121318[idx].b != 0xFF) {
        D_80120FC0[D_80121318[idx].b].tlb = 0xFF;
        D_80121318[idx].b = 0xFF;
    }
    return idx;
}
