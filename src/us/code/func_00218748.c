/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002612C8.c, func_002612C8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"
#include "globals/object.h"

B14Hdr *CISet__GetBlock(void *tree, s32 id);
s32 Anim_DecodeTracks(void *a, void *b, s32 c, s32 d, s32 e, s32 f);
extern f32 D_800E6790;
extern s32 D_800E6794;
extern s32 D_800E6798;
s32 Anim_DecodeAnimation(void *a, void *b) {
    s32 r;
    B14Hdr *h = CISet__GetBlock(a, 0);
    void *a1 = CISet__GetBlock(a, 1);
    void *a2 = CISet__GetBlock(a, 2);
    void *b1 = CISet__GetBlock(b, 1);
    void *b2 = CISet__GetBlock(b, 2);
    D_800E6798 = 1;
    D_800E6794 = 0;
    sAnimRemapMin = -2.0e3f;
    sAnimRemapMax = 2.0e3f;
    D_800E6790 = h->fC;
    if (Anim_DecodeTracks(a1, b1, h->f14, 3, h->f0, h->f8) != 0) {
        D_800E6798 = 0;
        D_800E6794 = 1;
        sAnimRemapMin = -1.0f;
        sAnimRemapMax = 1.0f;
        D_800E6790 = h->fC;
        r = Anim_DecodeTracks(a2, b2, h->f10, 4, h->f0, h->f8) != 0;
    } else {
        r = 0;
    }
    return r;
}
