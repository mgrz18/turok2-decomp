/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_2.c, func_802AFD00_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void __CSPPostNextSeqEvent(void *arg0);
extern s32 __alCSeqNextDelta(s32 arg0, s32 *arg1);
extern void alEvtqPostEvent(ALEventQueue *, ALEvent *, s32);
void __CSPPostNextSeqEvent(void *arg0) {
    ALSeqPlayer *player = arg0;
    ALEvent sp10;
    s32 sp20;
    sptr field18;
    if (player->state == 1) {
        field18 = (sptr)player->target;
        if (field18 != 0 && (__alCSeqNextDelta(field18, &sp20) & 0xFF)) {
            sp10.type = 0;
            alEvtqPostEvent(&player->evtq, &sp10, sp20 * player->uspt);
        }
    }
}
