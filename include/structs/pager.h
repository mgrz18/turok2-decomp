#ifndef STRUCTS_PAGER_H
#define STRUCTS_PAGER_H

/* The resource pager, gPager (Rage Wars' src/SCHED/8023A284_*.CPP):
 * code and data regions that live on the cartridge are mapped at virtual
 * pages through 24 TLB slots. A TLB miss in a thread is caught
 * (VM_HandleTLBMiss), the faulting page is read in by the manager thread
 * (VM_ManagerThread) through the reader thread (VM_ReaderThread) into the least
 * recently used slot, and the thread is resumed. The slots are kept in a
 * use-ordered list (chain): its head (also the symbol gPager_chain_head) is the least
 * recently used, its tail (gPager_chain_tail) the last. tools/check_structs.py checks the
 * offsets. */

#include "common.h"
#include "ultra_types.h"

/* One TLB slot of the pager. */
typedef struct PageMapping {
    /* 0x0 */ struct PageMapping *next;
    /* 0x4 */ struct PageMapping *prev;
    /* 0x8 */ u16 id;                   /* the page mapped here, 0xFFFF if none */
    /* 0xA */ u8 index;
    /* 0xB */ u8 slot;                  /* the TLB slot, 0xFF if none */
    /* 0xC */ s32 age;
} PageMapping; /* size 0x10 */

/* The slots in the order they were used. */
typedef struct PageChain {
    /* 0x0 */ PageMapping *head;        /* the least recently used */
    /* 0x4 */ PageMapping *tail;
} PageChain; /* size 0x8 */

typedef struct PageEntry {
    /* 0x0 */ u16 id;
    /* 0x2 */ u8 team;
    /* 0x3 */ u8 slot;
} PageEntry; /* size 0x4 */

/* A run of virtual pages backed by the cartridge. */
typedef struct PageRegion {
    /* 0x00 */ struct PageRegion *next;
    /* 0x04 */ u16 start;               /* the first virtual page */
    /* 0x06 */ u16 pages;
    /* 0x08 */ u8 *source;              /* its cartridge address */
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 *map;                 /* page -> slot index, 0xFF if not resident */
    /* 0x14 */ s32 unk14;
} PageRegion; /* size 0x18 */

/* A request to the manager thread: read size bytes of source into the
 * slot of mapping for page, then resume thread. */
typedef struct PageJob {
    /* 0x00 */ s16 type;
    /* 0x02 */ u8 _pad2[2];
    /* 0x04 */ u8 *source;
    /* 0x08 */ s32 size;
    /* 0x0C */ u16 page;
    /* 0x0E */ u8 _padE[2];
    /* 0x10 */ PageMapping *mapping;
    /* 0x14 */ void *thread;
    /* 0x18 */ u8 *slot;
} PageJob; /* size 0x1C */

typedef struct Pager {
    /* 0x000 */ s32 requests;           /* TLB misses handled */
    /* 0x004 */ s32 misses;             /* pages read in */
    /* 0x008 */ u32 lastAddress;        /* the last faulting address */
    /* 0x00C */ u8 unkC[0x20 - 0xC];
    /* 0x020 */ s16 event;
    /* 0x022 */ u8 _pad22[2];
    /* 0x024 */ OSMesgQueue completion; /* gPager_completion */
    /* 0x03C */ OSMesg completionMsgs[0x21];
    /* 0x0C0 */ u8 thread[0x230];       /* the manager thread (VM_ManagerThread) */
    /* 0x2F0 */ u8 stack[0x200];        /* its stack */
    /* 0x4F0 */ u8 readerThread[0x230]; /* the reader thread (VM_ReaderThread) */
    /* 0x720 */ u8 readerStack[0x200];  /* gPager_readerStack is its top */
    /* 0x920 */ PageMapping slots[24];
    /* 0xAA0 */ OSMesgQueue jobs;
    /* 0xAB8 */ OSMesg jobMsgs[8];
    /* 0xAD8 */ OSMesgQueue pending;    /* gPager_pending */
    /* 0xAF0 */ OSMesg pendingMsgs[8];
    /* 0xB10 */ u8 channels[8][0x1C];
    /* 0xBF0 */ PageChain chain;        /* gPager_chain_head (head), gPager_chain_tail (tail) */
    /* 0xBF8 */ u8 lookup[0x80];
    /* 0xC78 */ PageEntry entries[24];
    /* 0xCD8 */ PageRegion regions;     /* gPager_regions: the first region */
    /* 0xCF0 */ s16 expanded;
    /* 0xCF2 */ s16 mapped;
    /* 0xCF4 */ u8 _padCF4[4];
} Pager; /* size 0xCF8 */

#endif
