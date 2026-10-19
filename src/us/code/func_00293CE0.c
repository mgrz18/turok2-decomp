/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_7.c, func_802B2D80_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00291034(void *, s16 *, s32);
void func_00293CE0(ALSndPlayer *arg0) {
    s32 base = (s32)arg0->sndState;
    if (((ALSoundState *) (base + (arg0->target * 0x30)))->state == 0) {
        ParamsRecAt8V1 p;
        p.f0 = 0;
        p.f4 = base + arg0->target * 0x30;
        func_00291034(&arg0->evtq, (s16 *)&p, 0);
    }
}
extern s32 func_00291034(void *, s16 *, s32);
