/* NON-MATCHING: 245 of 390 words differ (size 0x5b8 vs 0x618): the highlight's `bright = (b * 3) >> 1`
 * is folded to a constant here (cse skips the `if (hi < lo)` block and knows b = 0xFF), the ROM computes it at
 * run time and rebuilds the last rect's words; the extra hoisted -1 then shifts the loop's register allocation.
 * Everything before the highlight (globals as struct fields for aliasing, the cmd pointer for the 0x646400 band)
 * matches. */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCENE/802508E0_6.CPP, func_80252A44_de, with Turok 2's layouts. */
#include "common.h"
#include "gfx.h"

/* A span of the heap (list at D_800D1CD0). */
typedef struct HeapSpan {
    char pad0[0xC];
    struct HeapSpan *next; /* 0x0C */
    u32 size;              /* 0x10 */
    u32 free;              /* 0x14 */
} HeapSpan;

/* A tracked allocation (list at D_800D81F4). */
typedef struct HeapNode {
    u32 start;             /* 0x00 */
    u32 size;              /* 0x04 */
    s32 count;             /* 0x08 */
    s32 flags;             /* 0x0C */
    s32 stamp;             /* 0x10 */
    char pad14[0x10];
    struct HeapNode *next; /* 0x24 */
} HeapNode;

/* The list heads and the frame stamp are struct fields in the original: the
 * display-list stores (struct fields too) may alias them, so they are reloaded. */
typedef struct { HeapSpan *head; } HeapSpanList;
typedef struct { HeapNode *head; } HeapNodeList;
typedef struct { s32 value; } Counter;

extern s32 D_800D8DCC;
extern char D_800D8DB0[]; /* OSMesgQueue, the heap lock */
extern HeapSpanList D_800D1CD0;
extern HeapNodeList D_800D81F4;
extern Counter D_800D8DF0;

extern u32 func_0029DFF0(void);
extern void func_0029E010(u32);
extern s32 osRecvMesg(void *mq, void **msg, s32 flag);
extern s32 osSendMesg(void *mq, void *msg, s32 flag);
extern void func_00207A70(s32);
extern void func_00207FEC(s32);

#define SHIFTL(v, s, w) ((((u32) (v)) & ((0x01 << (w)) - 1)) << (s))
#define DL(a, b) { Gfx *_g = D_800D8E24++; _g->w0 = (a); _g->w1 = (b); }
#define FILLRECT(ulx, uly, lrx, lry) \
    DL(SHIFTL(0xF6, 24, 8) | SHIFTL(lrx, 14, 10) | SHIFTL(lry, 2, 10), SHIFTL(ulx, 14, 10) | SHIFTL(uly, 2, 10))
#define PRIMCOLOR(r, g, b, a) \
    DL(SHIFTL(0xFA, 24, 8), SHIFTL(r, 24, 8) | SHIFTL(g, 16, 8) | SHIFTL(b, 8, 8) | SHIFTL(a, 0, 8))
#define CMD_PRIMCOLOR(cmd, r, g, b, a) \
    { Gfx *_g = (cmd); _g->w0 = SHIFTL(0xFA, 24, 8); \
      _g->w1 = SHIFTL(r, 24, 8) | SHIFTL(g, 16, 8) | SHIFTL(b, 8, 8) | SHIFTL(a, 0, 8); }
#define SAT(x) ((s32) (x) < 0x100 ? (x) : 0xFF)

static __inline__ void acquire(void) {
    u32 token = func_0029DFF0();
    s32 count = D_800D8DCC + 1;
    D_800D8DCC = count;
    if (count != 1) {
        func_0029E010(token);
        osRecvMesg(D_800D8DB0, 0, 1);
    } else {
        func_0029E010(token);
    }
}

static __inline__ void release(void) {
    u32 token = func_0029DFF0();
    s32 count = D_800D8DCC - 1;
    D_800D8DCC = count;
    if (count != 0) {
        func_0029E010(token);
        osSendMesg(D_800D8DB0, 0, 1);
    } else {
        func_0029E010(token);
    }
}

/* Debug view of the heap: a grey box with one coloured band per tracked
 * allocation, scaled to the total heap size. */
void func_00204778(s32 unused, s32 left, s32 top, s32 right, s32 bottom) {
    HeapSpan *span;
    HeapNode *node;
    u32 total;
    s32 height;
    s32 pass;
    s32 select;
    s32 lo;
    s32 hi;
    s32 r;
    s32 g;
    s32 b;
    s32 bright;
    u32 base;
    Gfx *cmd;

    DL(0xE7000000, 0);
    DL(0xE3000A01, 0);
    func_00207A70(0x19);
    func_00207FEC(0x15);
    PRIMCOLOR(0xA0, 0xA0, 0xA0, 0x00);
    FILLRECT(left - 1, top - 1, right + 1, bottom + 1);
    func_00207FEC(0x1B);
    PRIMCOLOR(0xA0, 0xA0, 0xA0, 0xFF);
    FILLRECT(left, top, right, bottom);

    acquire();

    span = D_800D1CD0.head;
    base = 0x80000000;
    total = (u32) span - base;
    for (; span != 0; span = span->next) {
        total += span->size + span->free;
    }

    cmd = D_800D8E24++;
    CMD_PRIMCOLOR(cmd, 0x64, 0x64, 0x00, 0xFF);
    height = bottom - top;
    hi = bottom - ((u32) (((u32) D_800D1CD0.head - base) * height) / total) - 1;
    FILLRECT(left, hi, right, bottom + 1);

    for (pass = 0; pass < 2; pass++) {
        for (node = D_800D81F4.head; node != 0; node = node->next) {
            if (pass != 0) {
                select = (node->flags & 0x101) == 0x100;
            } else {
                select = (node->flags & 0x101) != 0x100;
            }
            if (select == 0) {
                continue;
            }

            if (node->flags & 1) { r = 255; g = 255; b = 0; }
            else if (node->flags & 0x100) { r = 255; g = 0; b = 0; }
            else if (node->flags & 0x600) { r = 0; g = 0; b = 255; }
            else { r = 0; g = 255; b = 0; }

            PRIMCOLOR(r, g, b, 0xFF);
            hi = bottom - ((u32) (((node->start + node->size) - base) * height) / total) - 1;
            lo = bottom - ((u32) (((node->start - base) - 0x20) * height) / total) - 1;
            if (hi < lo) {
                FILLRECT(left, hi, right, lo);
            }

            PRIMCOLOR(SAT((u32) r >> 1), SAT((u32) g >> 1), SAT((u32) b >> 1), 0xFF);
            FILLRECT(left, lo - 1, right, lo);

            if (!(node->flags & 0x702) && (u32) (D_800D8DF0.value - node->stamp) >= 5U) {
                b = 0xFF;
                PRIMCOLOR(255, 255, 255, 255);
                for (;;) {
                    if (hi >= lo) break;
                    FILLRECT(left + 8, hi, right, lo);
                    break;
                }
                bright = (b * 3) >> 1;
                PRIMCOLOR(255, 255, SAT(bright), 255);
                FILLRECT(left + 8, lo - 1, right, lo);
            }
        }
    }

    release();
}
