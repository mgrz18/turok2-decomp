/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SCHED/CODE_8023CBF8.CPP, Sched_FinishPageJob.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
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
struct EntryRecAt3;
struct EntryRecAt3 {
    u16 id;
    u8 team;
    u8 slot;
};
struct EntryRecAt3;
typedef struct EntryRecAt3 EntryRecAt3;
struct OSMesgQueue;
struct OSMesgQueue;
struct Channels;
typedef struct Channels Channels;
struct ResourcePageMapping;
struct ResourcePageMapping {
    struct ResourcePageMapping *next;
    struct ResourcePageMapping *prev;
    u16 id;
    union { u16 padA; struct { u8 index; u8 slot; }; };
    s32 fieldC;
};
struct Region;
struct Region {
    struct Region *next;
    u16 start;
    u16 pages;
    u8 (*source)[4096];
    s32 padC;
    u8 *map;
    s32 pad14;
};
struct Region;
struct Slot_func_8023B9C0_eu;
typedef struct Slot_func_8023B9C0_eu Slot_func_8023B9C0_eu;
struct ResourcePageMapping;
typedef struct ResourcePageMapping ResourcePageMapping;
struct Channels;
struct Channels {
    char pad0[0x1C];
};
struct Slot_func_8023B9C0_eu;
struct Slot_func_8023B9C0_eu {
    union {
        struct { s32 a; s32 b; char pad8[2]; u8 index; char padB[5]; };
        ResourcePageMapping chain;
    };
};
struct PagerCounters {
    s32 requests;
    s32 misses;
    u32 lastAddress;
    char padC[0x20 - 0xC];
};
struct PagerState;
struct PagerState {
    struct PagerCounters counters;
    s16 f20;
    char pad22[2];
    OSMesgQueue completion;
    char list0Data[0xC0 - 0x3C];
    char queue[0x2F0 - 0xC0];
    char pool[0x4F0 - 0x2F0];
    char queueData[0x920 - 0x4F0];
    Slot_func_8023B9C0_eu slots[24];
    OSMesgQueue jobs;
    char list1Data[0xAD8 - 0xAB8];
    OSMesgQueue pending;
    char list2Data[0xB10 - 0xAF0];
    Channels channels[8];
    ResourcePageMapping *head;
    union { s32 fBF4; ResourcePageMapping *tail; };
    u8 lookup[0x100];
    EntryRecAt3 entries[24];
    struct Region regions;
    s16 fD70;
    s16 fD72;
    char padD74[4];
};
struct Region;
typedef struct Region Region;
struct PagerState;
typedef struct PagerState PagerState;
typedef void *OSMesg;
extern s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
struct OSMesgQueue;
struct OSMesgQueue;
struct OSMesgQueue;
extern ResourcePageMapping *D_80121294;
extern void func_0028DBAC(s16 flags, ResourcePageMapping *item, s32 unused, u8 *out);
extern ResourcePageMapping *D_80121290;
typedef struct ResourcePageThread {
    u8 reserved0[0x12]; s16 state;
    u8 reserved14[3]; u8 slot;
    u8 reserved18[0x108]; u32 message, address;
} ResourcePageThread;
typedef struct ResourcePageJob {
    u32 reserved0; u8 *source; u32 size;
    u16 page; u8 reservedE[2];
    ResourcePageMapping *mapping;
    ResourcePageThread *thread;
    u8 *slot;
} ResourcePageJob;
extern PagerState D_801206A0;
extern char D_80121140[];
extern void func_0029E030(u32, s32);
extern void func_0029E0D0(u32, s32);
extern void func_0029BB10(void *);
static __inline__ void writeLink(ResourcePageMapping **where, ResourcePageMapping *value) { *where = value; }
void func_0028ED10(ResourcePageJob *req) {
    ResourcePageMapping *link;
    if (*req->slot == 0xFF) {
        link = req->mapping;
        writeLink(&link->next, 0);
        writeLink(&link->prev, D_801206A0.tail);
        if (D_801206A0.tail != 0) {
            D_801206A0.tail->next = link;
        }
        D_80121294 = link;
        if (D_80121290 == 0) {
            D_80121290 = link;
        }
        func_0028DBAC(req->page, req->mapping, req->thread->slot, req->slot);
        func_0029E030(req->page << 12, 0x1000);
        func_0029E0D0(req->page << 12, 0x1000);
    } else {
        link = req->mapping;
        writeLink(&link->prev, 0);
        writeLink(&link->next, D_801206A0.head);
        if (D_801206A0.head != 0) {
            D_801206A0.head->prev = link;
        }
        D_80121290 = link;
        if (D_80121294 == 0) {
            D_80121294 = link;
        }
    }
    req->thread->state = 2;
    func_0029BB10(req->thread);
    osSendMesg(D_80121140, req, 1);
}
