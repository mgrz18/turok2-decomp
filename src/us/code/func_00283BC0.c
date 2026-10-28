/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80243A80_6.CPP, func_80244FB4_de, with Turok 2's layouts. */
#include "common.h"

/* Cutscene/cue playback context (D_800C1BB0). */
typedef struct CueContext {
    u8 pad0[0x1C];
    f32 time;           /* 0x1C */
    f32 time20;         /* 0x20 */
    u8 pad24[0x2C - 0x24];
    f32 clipEnd;        /* 0x2C */
    f32 start;          /* 0x30 */
    u8 pad34[0x3C - 0x34];
    s32 state3C;        /* 0x3C */
    u8 pad40[0xB4 - 0x40];
    s32 fieldB4;        /* 0xB4 */
    u8 padB8[0xDC - 0xB8];
    s32 resourceDC;     /* 0xDC */
} CueContext;

extern CueContext *D_800C1BB0;
extern s32 D_8011ACA4;
extern f32 D_800B6D28;      /* frame time */

extern void func_0040FD6C(s32);
extern s32 func_00284174(void);
extern s32 func_00284240(void);
extern void func_002838A4(void);
extern void func_00412798(void);

s32 func_00283BC0(void) {
    s32 none;
    CueContext *ctx;
    f32 value;
    f32 limit;

    none = -1;
    if (D_800C1BB0->resourceDC == none) {
        return 0;
    }
    func_0040FD6C(D_800C1BB0->resourceDC);
    D_800C1BB0->resourceDC = none;
    if (func_00284174() != 0) {
        if (D_8011ACA4 != 0 || func_00284240() != 0) {
            D_800C1BB0->time20 = D_800C1BB0->time;
        } else {
            ctx = D_800C1BB0;
            ctx->time20 = *(volatile f32 *)&ctx->time;
            ctx->time += D_800B6D28 * 0.06666667f;
            if (ctx->start <= ctx->time) {
                func_002838A4();
            }
            value = D_800C1BB0->time;
            limit = D_800C1BB0->clipEnd;
            if (limit <= value) {
                D_800C1BB0->time = limit;
                if (D_800C1BB0->fieldB4 != 0) {
                    D_800C1BB0->state3C = 1;
                    D_800C1BB0->fieldB4 = 0;
                }
            }
        }
    }
    func_00412798();
    return 1;
}
