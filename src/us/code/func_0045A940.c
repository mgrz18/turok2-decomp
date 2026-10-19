/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_4.c, func_80409F6C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern Shared_World D_800F7078;
extern s32 D_80110054;
extern void func_0042C620(void *, int, int);
extern void func_00285A80(void *);
void func_0045A940(void *unused, void *object) {
    func_00285A80(object);
    func_0042C620(&D_800F7078, D_80110054, 2);
}
