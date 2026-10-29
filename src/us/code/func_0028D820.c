/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/8023A284_11.CPP, func_8023B9C0_eu,
 * with Turok 2's layouts. */

#include "common.h"

typedef struct OSMesgQueue {
    void *mtqueue;
    void *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
} OSMesgQueue;

typedef struct PageSlot {
    /* 0x0 */ struct PageSlot *next;
    /* 0x4 */ struct PageSlot *prev;
    /* 0x8 */ u16 id;
    /* 0xA */ u8 index;
    /* 0xB */ u8 slot;
    /* 0xC */ s32 fieldC;
} PageSlot;

typedef struct PageChain {
    PageSlot *head;
    PageSlot *tail;
} PageChain;

typedef struct PageEntry {
    /* 0x0 */ u16 id;
    /* 0x2 */ u8 team;
    /* 0x3 */ u8 slot;
} PageEntry;

typedef struct PageRegion {
    /* 0x00 */ struct PageRegion *next;
    /* 0x04 */ u16 start;
    /* 0x06 */ u16 pages;
    /* 0x08 */ u8 *source;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 *map;
} PageRegion;

/* The resource pager at D_801206A0 (0xCF8 bytes). */
typedef struct Pager {
    /* 0x000 */ u8 pad0[0x20];
    /* 0x020 */ s16 event;
    /* 0x024 */ OSMesgQueue completion;
    /* 0x03C */ void *list0Data[0x21];
    /* 0x0C0 */ u8 thread[0x230];           /* the manager thread (func_0028E7A4) */
    /* 0x2F0 */ u8 stack[0x200];            /* its stack */
    /* 0x4F0 */ u8 readerThread[0x230];     /* the reader thread (func_0028E8A4) */
    /* 0x720 */ u8 readerStack[0x200];
    /* 0x920 */ PageSlot slots[24];
    /* 0xAA0 */ OSMesgQueue jobs;
    /* 0xAB8 */ void *list1Data[8];
    /* 0xAD8 */ OSMesgQueue pending;
    /* 0xAF0 */ void *list2Data[8];
    /* 0xB10 */ u8 channels[8][0x1C];
    /* 0xBF0 */ PageChain chain;
    /* 0xBF8 */ u8 lookup[0x80];
    /* 0xC78 */ PageEntry entries[24];
    /* 0xCD8 */ PageRegion regions;
    /* 0xCEC */ u8 padCEC[4];
    /* 0xCF0 */ s16 expanded;
    /* 0xCF2 */ s16 mapped;
    /* 0xCF4 */ u8 padCF4[4];
} Pager;

extern Pager D_801206A0;
extern u8 D_14A000[];               /* the paged data in the ROM */
extern u8 D_3CAE0[];                /* its size */
extern char D_800D1CD8[];           /* the cartridge reader */

u32 func_00266B80(void);            /* osGetMemSize */
void func_0029E2F0(void);           /* osUnmapTLBAll */
void func_0028E9E0(PageChain *chain, PageSlot *slot);
void func_0029B030(OSMesgQueue *queue, void **msg, s32 count);
s32 osSendMesg(OSMesgQueue *queue, void *msg, s32 block);
void func_00201DBC(void *reader, u8 *source, void *size, u32 dest);
void func_0028EE40(s32 arg0, s32 arg1, s32 arg2, u32 addr, u32 arg4, s32 arg5);
void func_0029B950(s32 event, OSMesgQueue *queue, s16 *msg);
void func_00266C5C(void *dst, s32 value, s32 size);
void func_0029B060(void *thread, s32 id, void (*entry)(void), void *arg, void *sp, s32 pri);
void func_0029BB10(void *thread);
void func_0028E7A4(void);

static __inline__ void fill(void *dst, u8 value, s32 n) {
    u8 *p = dst;

    while (n--) {
        *p++ = value;
    }
}

/* Sets up the resource pager: clears it, resets its tables and lists, maps
 * the expansion pak when present, and starts the manager thread. */
void func_0028D820(void) {
    u32 extra;
    u32 i;

    extra = func_00266B80() + 0x7FF80000;
    if (extra < 0x80400000) {
        extra = 0;
    }
    fill(&D_801206A0, 0, sizeof(Pager));
    fill(D_801206A0.lookup, 0xFF, sizeof(D_801206A0.lookup));
    fill(D_801206A0.slots, 0xFF, sizeof(D_801206A0.slots));
    func_0029E2F0();
    for (i = 0; i < 24; i++) {
        D_801206A0.entries[i].id = 0xFFFF;
        D_801206A0.entries[i].team = 0xFF;
        D_801206A0.entries[i].slot = 0xFF;
    }
    D_801206A0.chain.tail = NULL;
    D_801206A0.chain.head = NULL;
    for (i = 0; i < 24; i++) {
        D_801206A0.slots[i].next = NULL;
        D_801206A0.slots[i].prev = NULL;
        func_0028E9E0(&D_801206A0.chain, &D_801206A0.slots[i]);
        D_801206A0.slots[i].index = i;
    }
    func_0029B030(&D_801206A0.completion, D_801206A0.list0Data, 0x21);
    func_0029B030(&D_801206A0.jobs, D_801206A0.list1Data, 8);
    func_0029B030(&D_801206A0.pending, D_801206A0.list2Data, 8);
    D_801206A0.regions.start = 0x400;
    D_801206A0.regions.pages = 0x80;
    D_801206A0.regions.map = D_801206A0.lookup;
    D_801206A0.regions.next = NULL;
    D_801206A0.regions.source = (u8 *)((u32)D_14A000 | 0xB0000000);
    D_801206A0.mapped = 0;
    if (func_00266B80() > 0x4FFFFF) {
        func_00201DBC(D_800D1CD8, D_14A000, D_3CAE0, extra);
    }
    if (extra != 0) {
        func_0028EE40(0, 0x7E000, 0x400000, extra + 0x80000000, extra + 0x80040000, 7);
        D_801206A0.expanded = 1;
        D_801206A0.mapped = 1;
    }
    for (i = 0; i < 8; i++) {
        osSendMesg(&D_801206A0.jobs, D_801206A0.channels[i], 1);
    }
    D_801206A0.event = 0;
    func_0029B950(0xC, &D_801206A0.completion, &D_801206A0.event);
    func_00266C5C(D_801206A0.stack, 0x63, 0x200);
    func_0029B060(D_801206A0.thread, 0x63, func_0028E7A4, NULL, D_801206A0.stack + 0x200, 0x93);
    func_0029BB10(D_801206A0.thread);
}
