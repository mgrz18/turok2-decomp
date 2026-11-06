/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028BBE8.c, func_0028BBE8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"

typedef struct { u8 pad0[0x13]; u8 unk13; } Src;
s32 *CISet__GetBlock(s32 *tbl, s32 idx);
void CISet__GetBlockSize(s32 *tbl, s32 idx);
s32 CScene__IsEventFired(CScene *lv, Src *s) {
    s32 bit = s->unk13;
    s32 idx = lv->m_nLevel;
    s32 *t = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(lv->m_pBitSections, 0), idx), 2);
    u8 *bits;
    s32 m;
    CISet__GetBlock(t, 0);
    CISet__GetBlockSize(t, 1);
    bits = (u8 *)CISet__GetBlock(t, 1);
    m = 1 << (bit & 7);
    return (bits[bit / 8] & m) != 0;
}
