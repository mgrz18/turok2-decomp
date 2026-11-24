/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/CODE_8023CBF8.CPP, func_8023CDD4_de, with Turok 2's layouts
 * (round 29, part r2: the request is a PageRegionRequest). */
#include "common.h"
#include "structs/pager.h"
#include "globals/pager.h"
#include "protos/ultra.h"

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
