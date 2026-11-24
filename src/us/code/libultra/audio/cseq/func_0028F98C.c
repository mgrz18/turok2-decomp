/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_10.c, __readVarLen.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

u8 __getTrackByte(ALCSeq *seq, u32 track); /* ABI: returns u8 (the host leaves the upper bits of the return register undefined) */
u32 __readVarLen(ALCSeq *seq, u32 track);
u32 __alCSeqGetTrackEvent(ALCSeq *seq, u32 track, ALEvent *event);
u32 __readVarLen(ALCSeq *arg0, u32 arg1) {
    sptr temp_v0;
    sptr var_s0;
    var_s0 = (u8) __getTrackByte(arg0, arg1);
    if (var_s0 & 0x80) {
        var_s0 &= 0x7F;
        do {
            temp_v0 = (u8) __getTrackByte(arg0, arg1);
            var_s0 = (var_s0 << 7) + (temp_v0 & 0x7F);
        } while (temp_v0 & 0x80);
    }
    return var_s0;
}
