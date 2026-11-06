/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_4.c, func_80409F2C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void PakMenu_ProbeOwnerPort(struct Shared_PakMenu *record);
extern void Pak_ProbeSlot(s32);
void Pak_ProbeSlot(s32);
extern s32 D_800C2028;
extern s32 D_80130990;
void PakMenu_ProbeOwnerPort(struct Shared_PakMenu *record) {
    s32 value;
    if (D_80130990 != 0) {
        value = D_800C2028;
    } else {
        value = record->owner->unk4;
    }
    Pak_ProbeSlot(value);
}
extern s32 D_80130990;
