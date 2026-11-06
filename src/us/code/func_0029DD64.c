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
extern void __osPiGetAccess(void);
extern void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 msgCount);
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
static __inline__ void create_access_queue(void) {
    extern Slot __osPiAccessQueue;
    extern OSMesg __osPiAccessBuf;
    extern s32 __osPiAccessQueueEnabled[]; /* local type: an array, not the u32 of globals/libultra.h; as a scalar GCC 2 orders the code differently */
    __osPiAccessQueueEnabled[0] = 1;
    osCreateMesgQueue(&__osPiAccessQueue, &__osPiAccessBuf, 1);
    osSendMesg(&__osPiAccessQueue, 0, 0);
}
void __osPiGetAccess(void) {
    extern Slot __osPiAccessQueue;
    extern s32 __osPiAccessQueueEnabled[]; /* local type: an array, not the u32 of globals/libultra.h; as a scalar GCC 2 orders the code differently */
    OSMesg token;
    if (!__osPiAccessQueueEnabled[0]) {
        create_access_queue();
    }
    osRecvMesg(&__osPiAccessQueue, &token, 1);
}
