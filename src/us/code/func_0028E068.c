/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/8023A284_15.CPP, func_8023C1F0_de, with Turok 2's layouts. */
#include "common.h"

typedef struct OSMesgQueue {
    void *mtqueue;
    void *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
} OSMesgQueue;

typedef struct PageMapping {
    /* 0x0 */ struct PageMapping *next;
    /* 0x4 */ struct PageMapping *prev;
    /* 0x8 */ u16 id;           /* page mapped here, 0xFFFF if none */
    /* 0xA */ u8 index;
    /* 0xB */ u8 slot;          /* TLB slot, 0xFF if none */
    /* 0xC */ s32 age;
} PageMapping;

/* A run of virtual pages backed by the cartridge. */
typedef struct Region {
    /* 0x00 */ struct Region *next;
    /* 0x04 */ u16 start;
    /* 0x06 */ u16 pages;
    /* 0x08 */ u8 *source;
    /* 0x0C */ s32 padC;
    /* 0x10 */ u8 *map;         /* page -> mapping index, 0xFF if not resident */
    /* 0x14 */ s32 pad14;
} Region;

/* A request to the resource manager thread. */
typedef struct PageJob {
    /* 0x00 */ s16 type;
    /* 0x04 */ u8 *source;
    /* 0x08 */ s32 size;
    /* 0x0C */ u16 page;
    /* 0x10 */ PageMapping *mapping;
    /* 0x14 */ void *thread;
    /* 0x18 */ u8 *slot;
} PageJob;

typedef struct FaultedThread {
    /* 0x000 */ u8 pad0[0x12];
    /* 0x012 */ s16 state;
    /* 0x014 */ u8 pad14[3];
    /* 0x017 */ u8 slot;
    /* 0x018 */ u8 pad18[0x120 - 0x18];
    /* 0x120 */ u32 cause;
    /* 0x124 */ u32 badVaddr;
} FaultedThread;

/* The resource pager at D_801206A0. */
typedef struct Pager {
    /* 0x000 */ s32 requests;
    /* 0x004 */ s32 misses;
    /* 0x008 */ u32 lastAddress;
    /* 0x00C */ u8 padC[0x920 - 0xC];
    /* 0x920 */ PageMapping slots[24];
    /* 0xAA0 */ OSMesgQueue jobs;
    /* 0xAB8 */ u8 padAB8[0xAD8 - 0xAB8];
    /* 0xAD8 */ OSMesgQueue pending;    /* D_80121178 */
    /* 0xAF0 */ u8 padAF0[0xBF0 - 0xAF0];
    /* 0xBF0 */ PageMapping *head;      /* D_80121290: least recently used */
    /* 0xBF4 */ PageMapping *tail;      /* D_80121294 */
    /* 0xBF8 */ u8 padBF8[0xCD8 - 0xBF8];
    /* 0xCD8 */ Region regions;
} Pager;

/* D_80121290/D_80121294 seen as the pair they are; the miss path reads the head through it. */
typedef struct PageList {
    PageMapping *head;
    PageMapping *tail;
} PageList;

extern Pager D_801206A0;
extern PageMapping *D_80121290;    /* D_801206A0.head */
extern PageMapping *D_80121294;    /* D_801206A0.tail */
extern OSMesgQueue D_80121178;
extern s32 D_800B7658;

extern FaultedThread *func_0029B140(void);     /* __osGetCurrFaultedThread */
extern u64 func_0029B170(void);                /* osGetTime */
extern void func_0028DBAC(s16 page, PageMapping *mapping, s32 slot, u8 *map);
extern void func_0028DEFC(PageMapping *mapping);   /* releases its TLB slot */
extern Region *func_0028EA40(u16 page);
extern s32 osRecvMesg(OSMesgQueue *queue, void **msg, s32 block);
extern s32 osSendMesg(OSMesgQueue *queue, void *msg, s32 block);
extern void func_0029E3B0(void);
extern void func_0029E230(s32);
extern void func_0028CF80(void);
extern void func_0029BB10(void *thread);

static __inline__ void writeLink(PageMapping **where, PageMapping *value) {
    *where = value;
}

/* Unlinks a mapping from the LRU chain. */
static __inline__ void detachPage(PageMapping *mapping) {
    if (mapping->prev != NULL) {
        mapping->prev->next = mapping->next;
    } else {
        D_801206A0.head = mapping->next;
        mapping->next->prev = NULL;
    }
    if (mapping->next != NULL) {
        mapping->next->prev = mapping->prev;
    } else {
        D_801206A0.tail = mapping->prev;
        mapping->prev->next = NULL;
    }
}

/* func_0028EA40, inlined here. */
static __inline__ Region *findRegion(u32 page) {
    Region *region;

    for (region = &D_801206A0.regions; region != NULL; region = region->next) {
        if (page >= region->start && page < region->start + region->pages) {
            break;
        }
    }
    return region;
}

/* TLB miss handler: maps the faulting page if it is resident, else queues a read of it; any other fault restarts the game. */
void func_0028E068(void) {
    FaultedThread *thread;
    Region *region;
    Region *owner;
    PageMapping *mapping;
    PageJob *job;
    u8 *slot;
    u32 page;
    u32 kind;
    u64 start;

    thread = func_0029B140();
    kind = thread->cause & 0x7C;
    if (kind == 4 || kind == 8) {
        D_801206A0.requests++;
        page = thread->badVaddr >> 12;
        region = findRegion(page);
        if (region != NULL) {
            D_801206A0.lastAddress = thread->badVaddr;
            slot = &region->map[page - region->start];
            if (*slot != 0xFF) {
                mapping = &D_801206A0.slots[*slot];
                if (mapping->slot == 0xFF) {
                    detachPage(mapping);
                    func_0028DBAC(page, mapping, thread->slot, slot);
                    writeLink(&mapping->next, NULL);
                    writeLink(&mapping->prev, D_801206A0.tail);
                    if (D_801206A0.tail != NULL) {
                        D_801206A0.tail->next = mapping;
                    }
                    D_80121294 = mapping;
                    if (D_80121290 == NULL) {
                        D_80121290 = mapping;
                    }
                }
                thread->state = 2;
                func_0029BB10(thread);
                return;
            }
            osRecvMesg(&D_801206A0.jobs, (void **)&job, 1);
            D_801206A0.misses++;
            job->source = region->source + ((page - region->start) << 12);
            job->size = 0x1000;
            mapping = ((PageList *)&D_80121290)->head;
            detachPage(mapping);
            func_0028DEFC(mapping);
            if (mapping->id != 0xFFFF) {
                owner = func_0028EA40(mapping->id);
                owner->map[mapping->id - owner->start] = 0xFF;
                mapping->id = 0xFFFF;
            }
            mapping->age = 0;
            job->mapping = mapping;
            job->page = page;
            job->slot = slot;
            job->thread = thread;
            osSendMesg(&D_80121178, job, 1);
            return;
        }
        /* fault outside the paged regions: wait two seconds, then restart */
        start = func_0029B170();
        do {
        } while ((f32)(((func_0029B170() - start) * 64) / 3000) * 0.000001f < 2.0f);
    } else {
        start = func_0029B170();
        do {
        } while ((f32)(((func_0029B170() - start) * 64) / 3000) * 0.000001f < 2.0f);
    }
    func_0029E3B0();
    func_0029E230(1);
    func_0028CF80();
    ((void (*)(void))0x80000400)();
    D_800B7658 = -1;
    thread->state = 2;
    func_0029BB10(thread);
}
