/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_2.c, func_802AFD00_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00290C60(void *arg0);
extern s32 func_0028F910(s32 arg0, s32 *arg1);
extern void func_00291034(ALEventQueue *, ALEvent *, s32);
void func_00290C60(void *arg0) {
    struct ALSeqPlayer *player = arg0;
    ALEvent sp10;
    s32 sp20;
    s32 field18;
    if (player->state == 1) {
        field18 = (s32)player->target;
        if (field18 != 0 && (func_0028F910(field18, &sp20) & 0xFF)) {
            sp10.type = 0;
            func_00291034(&player->evtq, &sp10, sp20 * player->uspt);
        }
    }
}
