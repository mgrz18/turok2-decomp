/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/8023A284_15.CPP, func_8023C1F0_de, with Turok 2's layouts. */
#include "common.h"
#include "ultra_types.h"
#include "structs/pager.h"


typedef struct FaultedThread {
    /* 0x000 */ u8 pad0[0x12];
    /* 0x012 */ s16 state;
    /* 0x014 */ u8 pad14[3];
    /* 0x017 */ u8 slot;
    /* 0x018 */ u8 pad18[0x120 - 0x18];
    /* 0x120 */ u32 cause;
    /* 0x124 */ u32 badVaddr;
} FaultedThread;

extern OSMesgQueue gPager_pending;
extern s32 D_800B7658;

extern FaultedThread *__osGetCurrFaultedThread(void);     /* __osGetCurrFaultedThread */
extern u64 osGetTime(void);                /* osGetTime */
extern void VM_MapTLBEntry(s16 page, PageMapping *mapping, s32 slot, u8 *map);
extern void VM_UnmapTLBEntry(PageMapping *mapping);   /* releases its TLB slot */
extern PageRegion *VM_FindRegion(u16 page);
extern s32 osRecvMesg(OSMesgQueue *queue, void **msg, s32 block);
extern s32 osSendMesg(OSMesgQueue *queue, void *msg, s32 block);
extern void osWritebackDCacheAll(void);
extern void osSetIntMask(s32);
extern void SavedPlayers_Arm(void);
extern void osStartThread(void *thread);

static __inline__ void writeLink(PageMapping **where, PageMapping *value) {
    *where = value;
}

/* Unlinks a mapping from the LRU chain. */
static __inline__ void detachPage(PageMapping *mapping) {
    if (mapping->prev != NULL) {
        mapping->prev->next = mapping->next;
    } else {
        gPager.chain.head = mapping->next;
        mapping->next->prev = NULL;
    }
    if (mapping->next != NULL) {
        mapping->next->prev = mapping->prev;
    } else {
        gPager.chain.tail = mapping->prev;
        mapping->prev->next = NULL;
    }
}

/* VM_FindRegion, inlined here. */
static __inline__ PageRegion *findRegion(u32 page) {
    PageRegion *region;

    for (region = &gPager.regions; region != NULL; region = region->next) {
        if (page >= region->start && page < region->start + region->pages) {
            break;
        }
    }
    return region;
}

/* TLB miss handler: maps the faulting page if it is resident, else queues a read of it; any other fault restarts the game. */
void VM_HandleTLBMiss(void) {
    FaultedThread *thread;
    PageRegion *region;
    PageRegion *owner;
    PageMapping *mapping;
    PageJob *job;
    u8 *slot;
    u32 page;
    u32 kind;
    u64 start;

    thread = __osGetCurrFaultedThread();
    kind = thread->cause & 0x7C;
    if (kind == 4 || kind == 8) {
        gPager.requests++;
        page = thread->badVaddr >> 12;
        region = findRegion(page);
        if (region != NULL) {
            gPager.lastAddress = thread->badVaddr;
            slot = &region->map[page - region->start];
            if (*slot != 0xFF) {
                mapping = &gPager.slots[*slot];
                if (mapping->slot == 0xFF) {
                    detachPage(mapping);
                    VM_MapTLBEntry(page, mapping, thread->slot, slot);
                    writeLink(&mapping->next, NULL);
                    writeLink(&mapping->prev, gPager.chain.tail);
                    if (gPager.chain.tail != NULL) {
                        gPager.chain.tail->next = mapping;
                    }
                    gPager_chain_tail = mapping;
                    if (gPager_chain_head == NULL) {
                        gPager_chain_head = mapping;
                    }
                }
                thread->state = 2;
                osStartThread(thread);
                return;
            }
            osRecvMesg(&gPager.jobs, (void **)&job, 1);
            gPager.misses++;
            job->source = region->source + ((page - region->start) << 12);
            job->size = 0x1000;
            mapping = ((PageChain *)&gPager_chain_head)->head;
            detachPage(mapping);
            VM_UnmapTLBEntry(mapping);
            if (mapping->id != 0xFFFF) {
                owner = VM_FindRegion(mapping->id);
                owner->map[mapping->id - owner->start] = 0xFF;
                mapping->id = 0xFFFF;
            }
            mapping->age = 0;
            job->mapping = mapping;
            job->page = page;
            job->slot = slot;
            job->thread = thread;
            osSendMesg(&gPager_pending, job, 1);
            return;
        }
        /* fault outside the paged regions: wait two seconds, then restart */
        start = osGetTime();
        do {
        } while ((f32)(((osGetTime() - start) * 64) / 3000) * 0.000001f < 2.0f);
    } else {
        start = osGetTime();
        do {
        } while ((f32)(((osGetTime() - start) * 64) / 3000) * 0.000001f < 2.0f);
    }
    osWritebackDCacheAll();
    osSetIntMask(1);
    SavedPlayers_Arm();
    ((void (*)(void))0x80000400)();
    D_800B7658 = -1;
    thread->state = 2;
    osStartThread(thread);
}
