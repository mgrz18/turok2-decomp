/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SDK/OSINT/802BEA40.CPP, func_802B99A4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
struct Slot;
typedef struct Slot Slot;
struct Slot;
struct Slot {
    char data[24];
};
extern void __osSiGetAccess(void);
extern void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 msgCount);
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
static __inline__ void create_access_queue(void) {
    extern Slot __osSiAccessQueue;
    extern OSMesg __osSiAccessBuf;
    extern s32 __osSiAccessQueueEnabled[]; /* local type: an array, not the u32 of globals/libultra.h; as a scalar GCC 2 orders the code differently */
    __osSiAccessQueueEnabled[0] = 1;
    osCreateMesgQueue(&__osSiAccessQueue, &__osSiAccessBuf, 1);
    osSendMesg(&__osSiAccessQueue, 0, 0);
}
void __osSiGetAccess(void) {
    extern Slot __osSiAccessQueue;
    extern s32 __osSiAccessQueueEnabled[]; /* local type: an array, not the u32 of globals/libultra.h; as a scalar GCC 2 orders the code differently */
    OSMesg token;
    if (!__osSiAccessQueueEnabled[0]) {
        create_access_queue();
    }
    osRecvMesg(&__osSiAccessQueue, &token, 1);
}
