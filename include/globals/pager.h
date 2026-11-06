#ifndef GLOBALS_PAGER_H
#define GLOBALS_PAGER_H

/* The VM pager (structs/pager.h) and the pages it maps. */

#include "structs/pager.h"

extern Pager gPager;
extern PageMapping *gPager_chain_head;    /* gPager.chain.head */
extern PageMapping *gPager_chain_tail;    /* gPager.chain.tail */
extern PageRegion gPager_regions;         /* gPager.regions, the head of the region list */

#endif
