/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025C4B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00274728(void *arg0, f32 arg1);
extern void func_00293E60(void *sound, s16 sample);
extern void func_00293E60(void *sound, s16 sample);
extern void func_00293E60(void *arg0, s16 arg1);
extern void func_00293DE0(void *arg0, f32 arg1);
void func_00274728(void *arg0, f32 arg1) {
    RecordCode8025A3EC *slot = arg0;
    OwnerCode8025A3EC8025B5F0 *owner;
    s32 temp_s0;
    void *temp_s0_2;
    f32 scaled;
    temp_s0 = slot->owner;
    owner = (OwnerCode8025A3EC8025B5F0 *)temp_s0;
    temp_s0_2 = owner->sound;
    func_00293E60(temp_s0_2, owner->samples[slot->index]);
    scaled = arg1 * (slot->f34);
    scaled = scaled * (slot->fB8);
    func_00293DE0(temp_s0_2, scaled);
}
