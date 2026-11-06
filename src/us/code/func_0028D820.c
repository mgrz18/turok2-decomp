/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/8023A284_11.CPP, func_8023B9C0_eu,
 * with Turok 2's layouts. */

#include "common.h"
#include "ultra_types.h"
#include "structs/pager.h"


extern u8 D_14A000[];               /* the paged data in the ROM */
extern u8 D_3CAE0[];                /* its size */
extern char gLoader[];           /* the cartridge reader */

u32 GetMemSize(void);            /* osGetMemSize */
void __osUnmapTLBAll(void);           /* osUnmapTLBAll */
void VM_ListAddTail(PageChain *chain, PageMapping *slot);
void osCreateMesgQueue(OSMesgQueue *queue, void **msg, s32 count);
s32 osSendMesg(OSMesgQueue *queue, void *msg, s32 block);
void CLoader__LoadSync(void *reader, u8 *source, void *size, u32 dest);
void func_0028EE40(s32 arg0, s32 arg1, s32 arg2, u32 addr, u32 arg4, s32 arg5);
void osSetEventMesg(s32 event, OSMesgQueue *queue, s16 *msg);
void memset(void *dst, s32 value, s32 size);
void osCreateThread(void *thread, s32 id, void (*entry)(void), void *arg, void *sp, s32 pri);
void osStartThread(void *thread);
void VM_ManagerThread(void);

static __inline__ void fill(void *dst, u8 value, s32 n) {
    u8 *p = dst;

    while (n--) {
        *p++ = value;
    }
}

/* Sets up the resource pager: clears it, resets its tables and lists, maps
 * the expansion pak when present, and starts the manager thread. */
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
    gPager.regions.source = (u8 *)((u32)D_14A000 | 0xB0000000);
    gPager.mapped = 0;
    if (GetMemSize() > 0x4FFFFF) {
        CLoader__LoadSync(gLoader, D_14A000, D_3CAE0, extra);
    }
    if (extra != 0) {
        func_0028EE40(0, 0x7E000, 0x400000, extra + 0x80000000, extra + 0x80040000, 7);
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
