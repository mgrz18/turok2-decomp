/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SDK/OSINT/802BEA40.CPP, func_802B99A4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"
extern void __osSiGetAccess(void);
static __inline__ void create_access_queue(void) {
    ((s32 *) &__osSiAccessQueueEnabled)[0] = 1;
    osCreateMesgQueue(&__osSiAccessQueue, __osSiAccessBuf, 1);
    osSendMesg(&__osSiAccessQueue, 0, 0);
}
void __osSiGetAccess(void) {
    OSMesg token;
    if (!((s32 *) &__osSiAccessQueueEnabled)[0]) {
        create_access_queue();
    }
    osRecvMesg(&__osSiAccessQueue, &token, 1);
}
