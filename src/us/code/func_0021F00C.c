/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_2.c, func_8024BC94_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0021AE6C(void *arg0);
extern void func_00211114(void *arg0, void *arg1);
extern void func_0020F85C(void *, void *, void *);
void *func_0021F00C(void *arg0, s32 unused1, struct IntPair arg2) {
    s32 sp10[4];
    s32 sp20[16];
    Triple sp60;
    func_0021AE6C(sp10);
    func_00211114(sp10, sp20);
    func_0020F85C(sp20, &arg2, &sp60);
    *(Triple *)arg0 = sp60;
    return arg0;
}
