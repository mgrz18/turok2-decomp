/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7B80_us_30.c, func_802B8CF0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int D_800B8960;
extern s32 D_800B8960;
extern Queue *func_002A3060(void);
extern s32 osJamMesg(Queue *, void *, s32);
extern s32 osSendMesg(Queue *, s32, s32);
s32 func_002A2FB0(OSIoMesg *entry, s32 subtype, s32 arg2, s32 arg3,
                   s32 arg4, s32 arg5, Opaque_OSMesgQueue *arg6) {
    s16 type;
    if (D_800B8960 == 0) {
        return -1;
    }
    type = 12;
    if (arg2 == 0) {
        type = 11;
    }
    entry->hdr.type = type;
    entry->hdr.pri = subtype;
    entry->hdr.retQueue = arg6;
    entry->dramAddr = (void *)arg4;
    entry->devAddr = arg3;
    entry->size = arg5;
    entry->piHandle = 0;
    if (subtype != 1) {
        return osSendMesg(func_002A3060(), (s32)entry, 0);
    }
    return osJamMesg(func_002A3060(), entry, 0);
}
