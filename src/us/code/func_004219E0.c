/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_4.c, func_80409F2C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_004219E0(struct Shared_PakMenu *record);
extern void func_00426480(s32);
void func_00426480(s32);
extern s32 D_800C2028;
extern s32 D_80130990;
void func_004219E0(struct Shared_PakMenu *record) {
    s32 value;
    if (D_80130990 != 0) {
        value = D_800C2028;
    } else {
        value = record->owner->unk4;
    }
    func_00426480(value);
}
extern s32 D_80130990;
