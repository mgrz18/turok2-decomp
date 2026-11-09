/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028DC18.c, func_0028DC18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"
#include "protos/iset.h"
#include "protos/scene.h"

s32 CScene__GetSavedBit(CScene *lv, s32 sel, s32 c, s32 b, s32 bit) {
    s32 *t;
    s32 *base;
    u8 *bits;
    s32 m;
    if (sel != 0) {
        base = lv->m_pBitSections;
    } else {
        base = lv->m_pBitSections2;
    }
    t = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(base, 0), b), c);
    CISet__GetBlock(t, 0);
    CISet__GetBlockSize(t, 1);
    bits = (u8 *)CISet__GetBlock(t, 1);
    m = 1 << (bit & 7);
    return (bits[bit / 8] & m) != 0;
}
