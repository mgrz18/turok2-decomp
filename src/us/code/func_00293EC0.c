/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/sndpstop.c, alSndpStop.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00291034(ALEventQueue *, void *, s32);
void func_00293EC0(ObjRecAt40V2 *arg0) {
    s32 new_var;
    Buf16 sp10;
    new_var = arg0->field40;
    sp10.count = 1;
    sp10.value = new_var + (arg0->field3C * 0x30);
    func_00291034(&arg0->events, &sp10, 0);
}
