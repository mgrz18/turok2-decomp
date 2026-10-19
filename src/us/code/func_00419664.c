/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E830_de.c, func_8043EA4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00288C5C(struct GameSession *object, int value);
extern s32 func_00419664(void *arg0, Outer8043E56C *arg1);
extern void func_00285A80(s32 arg0);
extern s32 D_800F6CB0;
extern s32 D_801308F8;
s32 func_00419664(void *arg0, Outer8043E56C *arg1) {
    func_00288C5C(&D_800F6CB0, 8);
    D_801308F8 = 0;
    func_00285A80(arg1->unk20->unk4);
    return 1;
}
