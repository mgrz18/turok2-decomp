/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/CODE_8023CBF8.CPP, func_8023CDD4_de, with Turok 2's layouts. */
#include "common.h"

/* Page region, followed by its page map. */
typedef struct Region {
    struct Region *next; /* 0x00 */
    u16 start;           /* 0x04 first page */
    u16 pages;           /* 0x06 */
    u8 pad8[0x8];
    u8 *map;             /* 0x10 */
    u8 pad14[0x4];
} Region; /* size 0x18 */

typedef struct {
    u8 pad0[0x4];
    s32 size;            /* 0x04 */
    Region *region;      /* 0x08 */
    void *mq;            /* 0x0C reply queue */
} RegionDesc;

s32 osSendMesg(void *mq, void *msg, s32 flag);

extern Region D_80121378; /* region list head */

void func_0028EC54(RegionDesc *desc) {
    Region *p;
    Region *prev;
    Region *r;
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
