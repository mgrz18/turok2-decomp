/* FAKEMATCH: the `else { upper = lower; }` of the highlight is a dead store (upper is not read again in the
 * iteration). Flow deletes it after cse2, but until then the if/else stops cse's -fcse-skip-blocks path from
 * running past the `upper < lower` block, so `b` is not known to be 0xFF at the join and `(b * 3) >> 1` stays a
 * run-time multiply as in the ROM (and the two -1 words stay separate, unhoisted constants). The original
 * construct that did this is unknown; no asm, no register pins, default flags. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002529E4.c, func_002529E4,
 * with Turok 2's layouts (that version needs empty asms and a pinned $t1; here the colour chain gives the
 * allocation order and the dead else replaces the asms). */
#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "protos/cache.h"
#include "protos/ultra.h"

/* A tracked allocation (list at gCache_memLockedList). */
typedef struct HeapNode HeapNode;
struct HeapNode {
    /* 0x00 */ u32 start;
    /* 0x04 */ u32 size;
    /* 0x08 */ s32 count;
    /* 0x0C */ u32 flags;
    /* 0x10 */ u32 stamp;
    /* 0x14 */ u8 pad14[0x10];
    /* 0x24 */ HeapNode *next;
};

/* A span of the heap (list at gHeap_usedHead). */
typedef struct HeapSpan HeapSpan;
struct HeapSpan {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ HeapSpan *next;
    /* 0x10 */ u32 size;
    /* 0x14 */ u32 free;
};

extern char gCache_mutex[]; /* OSMesgQueue, the heap lock */
extern s32 gCache_mutex_count;    /* heap lock depth */
extern u32 gCache_ageCount[];  /* frame stamp */
extern HeapSpan *gHeap_usedHead[];
extern HeapNode *gCache_memLockedList[];
extern u32 __osDisableInt(void);   /* __osDisableInt */
extern void __osRestoreInt(u32);   /* __osRestoreInt */
extern void Render_SetCombineMode(s32);
extern void Render_SetRenderMode(s32);


/* gDPFillRectangle with ulx masked as a shift pair ((x << 22) >> 22): combine folds it back to the
 * `andi 0x3FF`, but at loop.c time it is two movables, which keeps left's (x & 0x3FF) << 14 inside the
 * pass loop as in the ROM (Rage Wars FINDINGS s703 no. 11). */
#define RECTA(pkt, ulx, uly, lrx, lry) \
    GFX_W(pkt, _SHIFTL(G_FILLRECT, 24, 8) | _SHIFTL(lrx, 14, 10) | _SHIFTL(lry, 2, 10), \
          ((((unsigned int) (ulx) << 22) >> 22) << 14) | _SHIFTL(uly, 2, 10))
/* gDPSetPrimColor with the colour word given as one expression. */
#define PRIMC(pkt, wd) GFX_W(pkt, _SHIFTL(G_SETPRIMCOLOR, 24, 8), wd)
/* Half of a colour component, clamped to 0xFF. */
#define HALF(c) ((((s32) ((c) >> 1)) < 0x100) ? ((s32) ((c) >> 1)) : 0xFF)

/* Debug view of the heap: a grey box with one band per tracked allocation, scaled to the total heap
 * size (yellow = locked, red = bit 8, blue = bits 9/10, else green) with a half-bright edge line; nodes
 * not touched for 5 or more frames get a white overlay with a brighter edge. */
void CCache__DrawDebug(s32 unused, s32 left, s32 top, s32 right, s32 bottom) {
    u32 mask;
    u32 mask2;
    HeapSpan *span;
    HeapNode *node;
    u32 total;
    s32 height;
    s32 upper;
    s32 lower;
    s32 pass;
    u32 r;
    u32 g;
    u32 b;
    u32 base;
    u32 rg;
    s32 select;

    gDPPipeSync(gDList++);
    gDPSetCycleType1Cycle(gDList++);
    Render_SetCombineMode(0x19);
    Render_SetRenderMode(0x15);
    gDPSetPrimColor(gDList++, 0, 0, 0xA0, 0xA0, 0xA0, 0x00);
    gDPFillRectangle(gDList++, left - 1, top - 1, right + 1, bottom + 1);
    Render_SetRenderMode(0x1B);
    gDPSetPrimColor(gDList++, 0, 0, 0xA0, 0xA0, 0xA0, 0xFF);
    gDPFillRectangle(gDList++, left, top, right, bottom);

    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(gCache_mutex, 0, 1);
    } else {
        __osRestoreInt(mask);
    }

    base = 0x80000000;
    span = gHeap_usedHead[0];
    total = (u32) span - base;
    for (; span != 0; span = span->next) {
        total += span->size + span->free;
    }

    gDPSetPrimColor(gDList++, 0, 0, 0x64, 0x64, 0x00, 0xFF);
    upper = (bottom - (u32) (((u32) gHeap_usedHead[0] - base) * (bottom - top)) / total) - 1;
    height = bottom - top;
    RECTA(gDList++, left, upper, right, bottom + 1);

    for (pass = 0; pass < 2; pass++) {
        for (node = gCache_memLockedList[0]; node != 0; node = node->next) {
            if (pass != 0) {
                select = (node->flags & 0x101) == 0x100;
            } else {
                select = (node->flags & 0x101) != 0x100;
            }
            if (select) {
                if (node->flags & 1) { r = 0xFF; g = 0xFF; b = 0; }
                else if (node->flags & 0x100) { r = 0xFF; g = 0; b = 0; }
                else if (node->flags & 0x600) { r = 0; g = 0; b = 0xFF; }
                else { r = 0; g = 0xFF; b = 0; }

                gDPSetPrimColor(gDList++, 0, 0, r, g, b, 0xFF);
                upper = (bottom - (u32) (((node->start + node->size) - base) * height) / total) - 1;
                lower = (bottom - (u32) (((node->start - base) - 0x20) * height) / total) - 1;
                if (upper < lower) {
                    RECTA(gDList++, left, upper, right, lower);
                }
                gDPSetPrimColor(gDList++, 0, 0, HALF(r), HALF(g), HALF(b), 0xFF);
                RECTA(gDList++, left, lower - 1, right, lower);

                if (!(node->flags & 0x702)) {
                    b = 0xFF;
                    if (gCache_ageCount[0] - node->stamp >= 5) {
                        gDPSetPrimColor(gDList++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
                        if (upper < lower) {
                            gDPFillRectangle(gDList++, left + 8, upper, right, lower);
                        } else {
                            upper = lower;
                        }
                        /* 1.5 times the highlight colour, clamped; rg in a variable keeps fold from merging
                         * its constant with the 0xFF (the ROM has lui 0xFFFF / or / ori 0xFF). */
                        rg = 0xFFFF0000;
                        PRIMC(gDList++, ({
                            s32 bright = ((s32) b * 3) >> 1;
                            bright < 0x100 ? (rg | ((bright & 0xFF) << 8)) | 0xFF : 0xFFFFFFFF;
                        }));
                        do { } while (0); /* sched1 barrier: keeps the colour store before the next lw */
                        gDPFillRectangle(gDList++, left + 8, lower - 1, right, lower);
                    }
                }
            }
        }
    }

    mask2 = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(mask2);
        osSendMesg(gCache_mutex, 0, 1);
    } else {
        __osRestoreInt(mask2);
    }
}
