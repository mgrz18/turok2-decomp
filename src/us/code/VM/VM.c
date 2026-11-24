/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/8023A284_11.CPP, func_8023B9C0_eu,
 * with Turok 2's layouts. */

#include "common.h"
#include "ultra_types.h"
#include "structs/pager.h"
#include "globals/pager.h"
#include "globals/sysrc.h"
#include "protos/ultra.h"
#include "protos/vm.h"

extern u8 gVirtualRomStart[];       /* ROM 0x14A000: the virtual segment (VM module 1 first) */
extern u8 gVirtualModule1Size[];    /* 0x3CAE0: VM module 1's size, an absolute symbol */
u32 GetMemSize(void);            /* osGetMemSize */
void __osUnmapTLBAll(void);           /* osUnmapTLBAll */
void VM_ListAddTail(PageChain *chain, PageMapping *node);
void CLoader__LoadSync(CLoader *thisx, uptr address, sptr length, void *dest);
void memset(void *dst, s32 value, sptr size);
void VM_ManagerThread(void);

static __inline__ void fill(void *dst, u8 value, s32 n) {
    u8 *p = dst;

    while (n--) {
        *p++ = value;
    }
}

/* Sets up the resource pager: clears it, resets its tables and lists, maps
 * the expansion pak when present, and starts the manager thread. */
#ifndef TARGET_PORT /* the port's in port/ (port/cmake: VM_Initialize) */
void VM_Initialize(void) {
    u32 extra;
    u32 i;

    extra = GetMemSize() + 0x7FF80000;
    if (extra < 0x80400000) {
        extra = 0;
    }
    fill(&gPager, 0, sizeof(Pager));
    fill(gPager.lookup, 0xFF, sizeof(gPager.lookup));
    fill(gPager.slots, 0xFF, sizeof(gPager.slots));
    __osUnmapTLBAll();
    for (i = 0; i < 24; i++) {
        gPager.entries[i].id = 0xFFFF;
        gPager.entries[i].team = 0xFF;
        gPager.entries[i].slot = 0xFF;
    }
    gPager.chain.tail = NULL;
    gPager.chain.head = NULL;
    for (i = 0; i < 24; i++) {
        gPager.slots[i].next = NULL;
        gPager.slots[i].prev = NULL;
        VM_ListAddTail(&gPager.chain, &gPager.slots[i]);
        gPager.slots[i].index = i;
    }
    osCreateMesgQueue(&gPager.completion, gPager.completionMsgs, 0x21);
    osCreateMesgQueue(&gPager.jobs, gPager.jobMsgs, 8);
    osCreateMesgQueue(&gPager.pending, gPager.pendingMsgs, 8);
    gPager.regions.start = 0x400;
    gPager.regions.pages = 0x80;
    gPager.regions.map = gPager.lookup;
    gPager.regions.next = NULL;
    gPager.regions.source = (u8 *)((uptr)gVirtualRomStart | 0xB0000000);
    gPager.mapped = 0;
    if (GetMemSize() > 0x4FFFFF) {
        CLoader__LoadSync(&gLoader, (uptr)gVirtualRomStart, (sptr)gVirtualModule1Size, (void *)extra);
    }
    if (extra != 0) {
        func_0028EE40(0, 0x7E000, (void *)0x400000, extra + 0x80000000, extra + 0x80040000, 7);
        gPager.expanded = 1;
        gPager.mapped = 1;
    }
    for (i = 0; i < 8; i++) {
        osSendMesg(&gPager.jobs, gPager.channels[i], 1);
    }
    gPager.event = 0;
    osSetEventMesg(0xC, &gPager.completion, &gPager.event);
    memset(gPager.stack, 0x63, 0x200);
    osCreateThread(gPager.thread, 0x63, VM_ManagerThread, NULL, gPager.stack + 0x200, 0x93);
    osStartThread(gPager.thread);
}
#endif

/* The next TLB slot to use, round-robin from gPager.mapped; the pages it held lose it. */
s32 VM_AcquireNextTLB(void) {
    s16 *cursor = &gPager_expanded; /* gPager.expanded, then gPager.mapped */
    s16 idx = cursor[0]++;
    if (cursor[0] >= 0x18) {
        cursor[0] = cursor[1];
    }
    if (gPager_entries[idx].team != 0xFF) {
        gPager_slots[gPager_entries[idx].team].slot = 0xFF;
        gPager_entries[idx].team = 0xFF;
    }
    if (gPager_entries[idx].slot != 0xFF) {
        gPager_slots[gPager_entries[idx].slot].slot = 0xFF;
        gPager_entries[idx].slot = 0xFF;
    }
    return idx;
}

static __inline__ void m03_unlink(PageMapping *n) {
    if (n->prev != NULL) {
        n->prev->next = n->next;
    } else {
        gPager.chain.head = n->next;
        n->next->prev = NULL;
    }
    if (n->next != NULL) {
        n->next->prev = n->prev;
    } else {
        gPager.chain.tail = n->prev;
        n->prev->next = NULL;
    }
}

static __inline__ void m03_push(PageMapping *n) {
    n->next = NULL;
    n->prev = gPager.chain.tail;
    if (gPager.chain.tail != NULL) {
        gPager.chain.tail->next = n;
    }
    gPager.chain.tail = n;
    if (gPager.chain.head == NULL) {
        gPager.chain.head = n;
    }
}

/* Maps virtual page `page` to mapping n's frame: an even/odd TLB pair shares one entry, so the
 * entry of its twin page (pair: the region's page -> mapping table) is reused when it has one. */
void VM_MapTLBEntry(s16 page, PageMapping *n, s32 unused, u8 *pair) {
    u8 *p = pair;
    s32 idx;
    PageMapping *q;
    if (page & 1) {
        p = pair - 1;
    }
    if (p[0] != 0xFF && gPager_slots[p[0]].slot != 0xFF) {
        idx = gPager_slots[p[0]].slot;
        gPager_entries[(s16)idx].slot = n->index;
    } else if (p[1] != 0xFF && gPager_slots[p[1]].slot != 0xFF) {
        idx = gPager_slots[p[1]].slot;
        gPager_entries[(s16)idx].team = n->index;
    } else {
        idx = VM_AcquireNextTLB();
        if (page & 1) {
            gPager_entries[(s16)idx].slot = n->index;
            if (p[0] != 0xFF) {
                q = &gPager_slots[p[0]];
                gPager_entries[(s16)idx].team = q->index;
                q->slot = idx;
                m03_unlink(q);
                m03_push(q);
            }
        } else {
            gPager_entries[(s16)idx].team = n->index;
            if (p[1] != 0xFF) {
                q = &gPager_slots[p[1]];
                gPager_entries[(s16)idx].slot = q->index;
                q->slot = idx;
                m03_unlink(q);
                m03_push(q);
            }
        }
        gPager_entries[(s16)idx].id = page & 0xFFFE;
    }
    n->id = page;
    n->slot = idx;
    *pair = n->index;
    func_0028EE40((s16)idx, 0, (void *)(gPager_entries[(s16)idx].id << 12),
             gPager_entries[(s16)idx].team != 0xFF ? ((u32)gVMPages & 0x3FFFFFF) + (gPager_entries[(s16)idx].team << 12) : -1,
             gPager_entries[(s16)idx].slot != 0xFF ? ((u32)gVMPages & 0x3FFFFFF) + (gPager_entries[(s16)idx].slot << 12) : -1, 7);
}

/* Releases mapping's half of its TLB entry; the entry is unmapped when its twin page has none. */
void VM_UnmapTLBEntry(PageMapping *mapping) {
    s32 slot;
    if (mapping->slot == 0xFF) {
        return;
    }
    if (mapping->id & 1) {
        gPager_entries[mapping->slot].slot = 0xFF;
        slot = mapping->slot;
        if (gPager_entries[slot].team == 0xFF) {
            goto unmap;
        }
        func_0028EE40(slot, 0, (void *)(gPager_entries[slot].id << 12),
                      ((sptr)&gVMPages & 0x3FFFFFF) + (gPager_entries[slot].team << 12), -1, 7);
    } else {
        gPager_entries[mapping->slot].team = 0xFF;
        slot = mapping->slot;
        if (gPager_entries[slot].slot == 0xFF) {
        unmap:
            func_0028EEC0(slot);
            gPager_entries[mapping->slot].id = 0xFFFF;
        } else {
            func_0028EE40(slot, 0, (void *)(gPager_entries[slot].id << 12), -1,
                          ((sptr)&gVMPages & 0x3FFFFFF) + (gPager_entries[slot].slot << 12), 7);
        }
    }
    mapping->slot = 0xFF;
}

extern s32 D_800B7658;
extern FaultedThread *__osGetCurrFaultedThread(void);     /* __osGetCurrFaultedThread */
extern void osSetIntMask(s32);
extern void SavedPlayers_Arm(void);

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

/* Byte-offset field access: a struct member reference here reorders the
 * stores below. */
#define FIELD_AT(ptr, type, T, field) (*(type *)((s8 *)(ptr) + (s32) &((T *)0)->field))

void VM_Age(void) {
    PageRegion *var_v1;
    s32 var_a0_2;
    s32 var_a1;
    u16 temp_v0;
    u32 *var_a0;
    u32 temp_v1;
    u32 var_a2;
    u32 var_a3;

    var_a2 = 0;
    var_a3 = 0x80000000;
    var_a1 = 0;
    var_a0 = (u32 *) &gPager.slots[0].age;
    do {
        temp_v1 = *var_a0;
        if (var_a2 < temp_v1) {
            var_a2 = temp_v1;
        }
        if (temp_v1 < var_a3) {
            var_a3 = temp_v1;
        }
        *var_a0 = temp_v1 + 1;
        var_a1 += 1;
        var_a0 = (__typeof__(var_a0))((s8 *)var_a0 + 0x10);
    } while (var_a1 < 0x18);
    var_v1 = &gPager_regions;
    var_a0_2 = 0;
    if (&gPager_regions != NULL) {
        do {
            temp_v0 = var_v1->pages;
            var_v1 = var_v1->next;
            var_a0_2 += temp_v0 << 0xC;
        } while (var_v1 != NULL);
    }
    gPager.statRegionBytes = var_a0_2;     /* total pages << 12 */
    gPager.statMaxAge = var_a2;
    gPager.statMinAge = var_a3;
    gPager.statChainAge = FIELD_AT(gPager_chain_head, s32, PageMapping, age);
}

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: osMapTLBRdb) */
void osMapTLBRdb(void) {
    func_0028EE40(0x1F, 0x1FE000, (void *)0x200000, 0, 0x100000, 7);
}
#endif

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_13.c, func_8023C78C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/cache.h"

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: VM_Alloc) */
s32 VM_Alloc(s32 arg0, s32 arg1_unused, s32 arg2)
{
  char sp10[0x18];
  int new_var2;
  Block10 blk;
  s32 sp38;
  u32 temp_s1;
  s32 sp3C;
  u32 rounded;
  void **temp_v0;
  Draw *temp_v1;
  int new_var;
  temp_s1 = ((u32) (arg0 + 0xFFF)) >> 0xC;
  new_var = (temp_s1 + 1) & (~1);
  {
    osCreateMesgQueue(sp10, (&sp38), 1);
    blk.f28 = 3;
    blk.f2C = arg0;
    blk.f34 = (void **) sp10;
    new_var2 = temp_s1 * 4;
    rounded = new_var + 0x18;
    temp_v0 = CCache__Allocate(0, new_var2 + rounded, 3, "Virtual Region");
    temp_v1 = *temp_v0;
    blk.f30 = temp_v1;
    temp_v1->model = temp_v0;
  }
  ((Draw *)blk.f30)->allocationMode = arg2;
  osSendMesg(&gPager_completion, &blk.f28, 1);
  osRecvMesg(sp10, &sp3C, 1);
  return sp3C;
}
#endif
void VM_Free(s32 arg0);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: VM_Free) */
void VM_Free(s32 arg0) {
    char sp10[0x18];
    Block_func_8023C85C_de block;
    s32 sp38;
    void *sp3C;
    osCreateMesgQueue(sp10, &sp38, 1);
    block.type = 2;
    block.value = arg0;
    block.data = sp10;
    osSendMesg(&gPager_completion, &block, 1);
    osRecvMesg(sp10, &sp3C, 1);
    CCache__Deallocate(0, ((Draw *)(sp3C))->model);
}
#endif

/* Copies the pager's counters out. */
void VM_GetStats(PagerStats *out) {
    *out = *(PagerStats *)&gPager;
}
void VM_FinishPageJob(PageJob *req);
void VM_FreeRegion(PageFreeRequest *req);
void VM_AllocRegion(PageRegionRequest *desc);
void VM_ReaderThread(void *unused);

/* Resource manager thread: starts the reader thread, then dispatches requests forever. */
void VM_ManagerThread(void) {
    void *request;

    memset(gPager_readerStack, 0x62, 0x200);
    osCreateThread(gPager_readerStack - 0x230, 0x62, VM_ReaderThread, NULL, gPager_readerStack + 0x200, 0x94);
    osStartThread(gPager_readerStack - 0x230);
    for (;;) {
        osRecvMesg(&gPager.completion, &request, 1);
        switch (((PageJob *)request)->type) {
            case 0:
                VM_HandleTLBMiss();
                break;
            case 1:
                VM_FinishPageJob(request);
                break;
            case 3:
                VM_AllocRegion((PageRegionRequest *)request);
                break;
            case 2:
                VM_FreeRegion((PageFreeRequest *)request);
                break;
        }
    }
}

/* Reader thread: reads each pending page from the cartridge into its slot of
 * the page array and hands the job back to the manager. */
void VM_ReaderThread(void *unused) {
    OSMesgQueue queue;
    void *queueMessage;
    void *message;

    osCreateMesgQueue(&queue, &queueMessage, 1);
    for (;;) {
        osRecvMesg(&gPager.pending, &message, 1);
        if (*((PageJob *)message)->slot == 0xFF) {
            CLoader__LoadSync(&gLoader, (uptr)((PageJob *)message)->source, (((PageJob *)message)->size + 1) & ~1,
                          gVMPages + (((PageJob *)message)->mapping->index << 12));
            osInvalDCache(gVMPages + (((PageJob *)message)->mapping->index << 12), 0x1000);
            osInvalICache(gVMPages + (((PageJob *)message)->mapping->index << 12), 0x1000);
        }
        ((PageJob *)message)->type = 1;
        osSendMesg(&gPager.completion, message, 1);
    }
}

/* Unlinks a mapping from a doubly linked chain. */
void VM_ListRemove(PageChain *list, PageMapping *node) {
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
        node->next->prev = NULL;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
        node->prev->next = NULL;
    }
}

/* Appends node to the chain. */
void VM_ListAddTail(PageChain *chain, PageMapping *node) {
    PageMapping *tail;

    node->next = NULL;
    node->prev = chain->tail;
    tail = chain->tail;
    if (tail != NULL) {
        tail->next = node;
    }
    chain->tail = node;
    if (chain->head == NULL) {
        chain->head = node;
    }
}

/* Pushes node at the head of the chain. */
void VM_ListAddHead(PageChain *chain, PageMapping *node) {
    PageMapping *head;

    node->prev = NULL;
    node->next = chain->head;
    head = chain->head;
    if (head != NULL) {
        head->prev = node;
    }
    chain->head = node;
    if (chain->tail == NULL) {
        chain->tail = node;
    }
}

/* The memory region containing page arg0. */
PageRegion *VM_FindRegion(u32 arg0) {
    PageRegion *var_v1;
    u16 temp_a1;

    var_v1 = &gPager_regions;
    if (&gPager_regions != NULL) {
loop_1:
        temp_a1 = var_v1->start;
        if ((arg0 < temp_a1) || (arg0 >= (u32) (temp_a1 + var_v1->pages))) {
            var_v1 = var_v1->next;
            if (var_v1 != NULL) {
                goto loop_1;
            }
        }
    }
    return var_v1;
}

/* Takes the least recently used mapping off the chain and frees its page. */
PageMapping *VM_ReclaimLRUPage(void) {
    PageMapping *entry;
    PageRegion *region;
    entry = gPager_chain_head;
    if (entry->prev != NULL) {
        entry->prev->next = *(PageMapping * volatile *)&entry->next;
    } else {
        gPager_chain_head = entry->next;
        (*(PageMapping * volatile *)&entry->next)->prev = NULL;
    }
    if (entry->next != NULL) {
        entry->next->prev = *(PageMapping * volatile *)&entry->prev;
    } else {
        gPager_chain_tail = entry->prev;
        (*(PageMapping * volatile *)&entry->prev)->next = NULL;
    }
    VM_UnmapTLBEntry(entry);
    if (entry->id != 0xFFFF) {
        region = VM_FindRegion(entry->id);
        region->map[entry->id - region->start] = 0xFF;
        entry->id = 0xFFFF;
    }
    entry->age = 0;
    return entry;
}

/* Unlinks the region at req->address, drops its resident pages and hands it back. */
void VM_FreeRegion(PageFreeRequest *req) {
    PageRegion *node;
    PageRegion *prev;
    u8 *slot;
    s32 i;
    u32 key;
    node = &gPager_regions;
    key = req->address >> 12;
    prev = 0;
    while (node != 0) {
        if (node->start == key) {
            break;
        }
        prev = node;
        node = node->next;
    }
    prev->next = node->next;
    slot = node->map;
    for (i = 0; i < node->pages; i++) {
        if (*slot != 0xFF) {
            VM_UnmapTLBEntry(&gPager_slots[*slot]);
            gPager_slots[*slot].id = 0xFFFF;
        }
        slot++;
    }
    osSendMesg(req->reply, node, 1);
}

/* Appends desc->region after the last region (on an even page), its pages not resident, and
 * replies with its virtual address. */
void VM_AllocRegion(PageRegionRequest *desc) {
    PageRegion *p;
    PageRegion *prev;
    PageRegion *r;
    uptr start;
    s32 pages;
    s32 count;
    u8 *map;
    s32 i;
    s32 fill;

    start = 0;
    pages = (desc->size + 0xFFF) / 0x1000;
    p = &gPager_regions;
    prev = 0;
    r = desc->region;
    r->map = (u8 *) (r + 1);
    count = (pages + 1) & ~1;
    for (; p != 0; p = p->next) {
        prev = p;
        start = p->start + p->pages;
    }
    if (start & 1) {
        start++;
    }
    i = count;
    r->start = start;
    r->pages = pages;
    r->next = 0;
    prev->next = r;
    map = r->map;
    fill = 0xFF;
    while (i-- != 0) {
        *map++ = fill;
    }
    osSendMesg(desc->reply, (void *) (start << 12), 1);
}

/* A page read in: maps it and resumes the faulting thread (or, when the page was mapped meanwhile,
 * returns the mapping to the head of the chain), then frees the job. */
void VM_FinishPageJob(PageJob *req) {
    PageMapping *link;
    if (*req->slot == 0xFF) {
        link = req->mapping;
        writeLink(&link->next, 0);
        writeLink(&link->prev, gPager.chain.tail);
        if (gPager.chain.tail != 0) {
            gPager.chain.tail->next = link;
        }
        gPager_chain_tail = link;
        if (gPager_chain_head == 0) {
            gPager_chain_head = link;
        }
        VM_MapTLBEntry(req->page, req->mapping, ((FaultedThread *)req->thread)->slot, req->slot);
        osInvalDCache((void *)(req->page << 12), 0x1000);
        osInvalICache((void *)(req->page << 12), 0x1000);
    } else {
        link = req->mapping;
        writeLink(&link->prev, 0);
        writeLink(&link->next, gPager.chain.head);
        if (gPager.chain.head != 0) {
            gPager.chain.head->prev = link;
        }
        gPager_chain_head = link;
        if (gPager_chain_tail == 0) {
            gPager_chain_tail = link;
        }
    }
    ((FaultedThread *)req->thread)->state = 2;
    osStartThread(req->thread);
    osSendMesg(&gPager_jobs, req, 1);
}
