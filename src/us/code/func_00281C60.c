/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80233930_de_30.c, func_80239DE8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void CShake__Set(struct RecordRecAt18 *record, f32 first, f32 second, f32 third, f32 value, s32 mode, Triple triple);
void CShake__Set(struct RecordRecAt18 *record, f32 first, f32 second, f32 third, f32 value, s32 mode,
                   Triple triple) {
    record->triple = triple;
    record->value = value;
    record->channels[0].mode = mode;
    record->channels[1].mode = mode;
    record->channels[2].mode = mode;
    record->channels[0].level = first * 0.1f;
    record->channels[1].level = second * 0.1f;
    record->channels[2].level = third * 0.1f;
}
