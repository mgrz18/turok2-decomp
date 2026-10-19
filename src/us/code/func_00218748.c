/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002612C8.c, func_002612C8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

B14Hdr *func_002017D4(void *tree, s32 id);
s32 func_00216604(void *a, void *b, s32 c, s32 d, s32 e, s32 f);
extern f32 D_800A59F8;
extern f32 D_800A59FC;
extern f32 D_800A5A00;
extern f32 D_800A5A04;
extern f32 D_800E6790;
extern s32 D_800E6794;
extern s32 D_800E6798;
extern f32 D_800E679C;
extern f32 D_800E67A0;
s32 func_00218748(void *a, void *b) {
    s32 r;
    B14Hdr *h = func_002017D4(a, 0);
    void *a1 = func_002017D4(a, 1);
    void *a2 = func_002017D4(a, 2);
    void *b1 = func_002017D4(b, 1);
    void *b2 = func_002017D4(b, 2);
    D_800E6798 = 1;
    D_800E6794 = 0;
    D_800E679C = D_800A59F8;
    D_800E67A0 = D_800A59FC;
    D_800E6790 = h->fC;
    if (func_00216604(a1, b1, h->f14, 3, h->f0, h->f8) != 0) {
        D_800E6798 = 0;
        D_800E6794 = 1;
        D_800E679C = D_800A5A00;
        D_800E67A0 = D_800A5A04;
        D_800E6790 = h->fC;
        r = func_00216604(a2, b2, h->f10, 4, h->f0, h->f8) != 0;
    } else {
        r = 0;
    }
    return r;
}
