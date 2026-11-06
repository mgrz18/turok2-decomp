#ifndef GLOBALS_PAGER_H
#define GLOBALS_PAGER_H

/* The VM pager (structs/pager.h) and the pages it maps. */

#include "structs/pager.h"

extern Pager gPager;
extern PageMapping *gPager_chain_head;    /* gPager.chain.head */
extern PageMapping *gPager_chain_tail;    /* gPager.chain.tail */
extern PageRegion gPager_regions;         /* gPager.regions, the head of the region list */

/* More fields with symbols of their own, and the pages */
extern OSMesgQueue gPager_completion;     /* gPager.completion */
extern OSMesgQueue gPager_jobs;           /* gPager.jobs */
extern OSMesgQueue gPager_pending;        /* gPager.pending */
extern u8 gPager_readerStack[0x200];     /* gPager.readerStack; its OSThread is 0x230 below */
extern PageMapping gPager_slots[24];     /* gPager.slots */
extern PageEntry gPager_entries[24];     /* gPager.entries */
extern s16 gPager_expanded;               /* gPager.expanded */
extern u8 gVMPages[];                     /* the page frames, 0x1000 bytes each */

#endif
