/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BAE40_de_22.c, func_802BB550_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"

void osSetEventMesg(s32 event, OSMesgQueue *mq, OSMesg msg) {
    extern void __osRestoreInt(u32);
    extern u32 __osDisableInt(void);
    u32 saveMask;
    saveMask = __osDisableInt();
    (&__osEventStateTab[event])->messageQueue = mq;
    (&__osEventStateTab[event])->message = msg;
    __osRestoreInt(saveMask);
}
