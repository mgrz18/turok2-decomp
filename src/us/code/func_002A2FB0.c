/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7B80_us_30.c, func_802B8CF0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
struct Queue;
struct Queue {
    char pad[0x14];
    s32 items[4];
};
struct Queue;
extern int D_800B8960;
struct OSIoMesg;
typedef struct OSIoMesg OSIoMesg;
struct OSIoMesgHdr;
typedef struct OSIoMesgHdr OSIoMesgHdr;
struct Opaque_OSMesgQueue;
typedef struct Opaque_OSMesgQueue Opaque_OSMesgQueue;
struct OSIoMesgHdr;
struct OSIoMesgHdr {
    u16 type;
    u8 pri;
    u8 status;
    Opaque_OSMesgQueue *retQueue;
};
struct OSIoMesg;
struct OSIoMesg {
    OSIoMesgHdr hdr;
    void *dramAddr;
    u32 devAddr;
    u32 size;
    void *piHandle;
};
typedef struct Queue Queue;
extern s32 D_800B8960;
extern Queue *func_002A3060(void);
extern s32 func_0029B5B0(Queue *, void *, s32);
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
    return func_0029B5B0(func_002A3060(), entry, 0);
}
