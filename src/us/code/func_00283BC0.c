/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80243A80_6.CPP, func_80244FB4_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/cinema.h"

extern Cinema *D_800C1BB0;
extern s32 D_8011ACA4;
extern f32 D_800B6D28;      /* frame time */

extern void func_0040FD6C(s32);
extern s32 func_00284174(void);
extern s32 func_00284240(void);
extern void func_002838A4(void);
extern void func_00412798(void);

s32 func_00283BC0(void) {
    s32 none;
    Cinema *ctx;
    f32 value;
    f32 limit;

    none = -1;
    if (D_800C1BB0->entryIndex == none) {
        return 0;
    }
    func_0040FD6C(D_800C1BB0->entryIndex);
    D_800C1BB0->entryIndex = none;
    if (func_00284174() != 0) {
        if (D_8011ACA4 != 0 || func_00284240() != 0) {
            D_800C1BB0->prevTime = D_800C1BB0->time;
        } else {
            ctx = D_800C1BB0;
            ctx->prevTime = *(volatile f32 *)&ctx->time;
            ctx->time += D_800B6D28 * 0.06666667f;
            if (ctx->startTime <= ctx->time) {
                func_002838A4();
            }
            value = D_800C1BB0->time;
            limit = D_800C1BB0->clipEnd;
            if (limit <= value) {
                D_800C1BB0->time = limit;
                if (D_800C1BB0->stopAtEnd != 0) {
                    D_800C1BB0->finished = 1;
                    D_800C1BB0->stopAtEnd = 0;
                }
            }
        }
    }
    func_00412798();
    return 1;
}
