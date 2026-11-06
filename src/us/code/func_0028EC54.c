/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/CODE_8023CBF8.CPP, func_8023CDD4_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/pager.h"

typedef struct {
    u8 pad0[0x4];
    s32 size;            /* 0x04 */
    PageRegion *region;      /* 0x08 */
    void *mq;            /* 0x0C reply queue */
} RegionDesc;

s32 osSendMesg(void *mq, void *msg, s32 flag);


void VM_AllocRegion(RegionDesc *desc) {
    PageRegion *p;
    PageRegion *prev;
    PageRegion *r;
    u32 start;
    s32 pages;
    s32 count;
    u8 *map;
    s32 i;
    s32 fill;

    start = 0;
    pages = (desc->size + 0xFFF) / 0x1000;
    p = &D_80121378;
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
    osSendMesg(desc->mq, (void *) (start << 12), 1);
}
