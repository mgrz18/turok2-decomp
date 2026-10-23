/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SDK/OSINT/802BEA40.CPP, func_802B99A4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef struct OSMesgQueue OSMesgQueue;
struct OSMesgQueue {
    void **mtqueue;
    void **fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
};
struct OSMesgQueue;
struct OSMesgQueue;
struct Slot;
typedef struct Slot Slot;
struct OSMesgQueue;
struct Slot;
struct Slot {
    char data[24];
};
struct OSMesgQueue;
extern void func_0029DD64(void);
typedef void *OSMesg;
extern void func_0029B030(OSMesgQueue *mq, OSMesg *msg, s32 msgCount);
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
static __inline__ void create_access_queue(void) {
    extern Slot D_80124DB8;
    extern OSMesg D_80124DB0;
    extern s32 D_800B8990[];
    D_800B8990[0] = 1;
    func_0029B030(&D_80124DB8, &D_80124DB0, 1);
    osSendMesg(&D_80124DB8, 0, 0);
}
void func_0029DD64(void) {
    extern Slot D_80124DB8;
    extern s32 D_800B8990[];
    OSMesg token;
    if (!D_800B8990[0]) {
        create_access_queue();
    }
    osRecvMesg(&D_80124DB8, &token, 1);
}
