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
    extern Slot D_80124DB8;
    extern OSMesg D_80124DB0;
    extern s32 D_800B8990[];
    D_800B8990[0] = 1;
    osCreateMesgQueue(&D_80124DB8, &D_80124DB0, 1);
    osSendMesg(&D_80124DB8, 0, 0);
}
void __osPiGetAccess(void) {
    extern Slot D_80124DB8;
    extern s32 D_800B8990[];
    OSMesg token;
    if (!D_800B8990[0]) {
        create_access_queue();
    }
    osRecvMesg(&D_80124DB8, &token, 1);
}
