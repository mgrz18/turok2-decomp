/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80243A80_6.CPP, func_802450CC_de, with Turok 2's layouts. */
#include "common.h"

typedef void (*CueCallback)(void);

/* Cutscene/cue playback context (D_800C1BB0). */
typedef struct CueContext {
    void *owner;            /* 0x00 */
    void *resource;         /* 0x04 */
    u8 pad08[0xC - 0x8];
    CueCallback onClose;    /* 0x0C */
    CueCallback callback10; /* 0x10 */
    u8 pad14[0x1C - 0x14];
    f32 time;               /* 0x1C */
    f32 time20;             /* 0x20 */
    u8 pad24[0x2C - 0x24];
    f32 clipEnd;            /* 0x2C */
    f32 start;              /* 0x30 */
    f32 end;                /* 0x34 */
    s32 active;             /* 0x38 */
    s32 state3C;            /* 0x3C */
    u8 pad40[0x44 - 0x40];
    s32 initialized44;      /* 0x44 */
    s32 closed;             /* 0x48 */
    u8 pad4C[0x5C - 0x4C];
    s32 unknown5C;          /* 0x5C */
    s32 unknown60;          /* 0x60 */
    f32 fade;               /* 0x64 */
    u8 pad68[0xB4 - 0x68];
    s32 fieldB4;            /* 0xB4 */
} CueContext;

typedef struct GameState {
    s32 busy;               /* 0x0000 */
    u8 pad4[0x1450 - 0x4];
    s32 paused;             /* 0x1450 */
} GameState;

extern CueContext *D_800C1BB0;
extern s32 D_800B6CE0;      /* last frame handled */
extern s32 D_800B6D18;      /* frame counter */
extern s32 D_8011ACA4;
extern f32 D_800B6D28;      /* frame time */
extern GameState D_801198A4;

extern s32 func_00283BC0(void);
extern s32 func_00284174(void);
extern s32 func_00284240(void);
extern void func_002838A4(void);
extern void func_00283A4C(void);
extern void func_00412798(void);
extern void func_00284468(s32);
extern void func_002844F0(void);
extern void func_002052D8(void *, void *);
extern void func_00275904(s32);
extern void func_00206068(s32);

void func_00283CD4(void) {
    CueContext *ctx;
    f32 value;
    f32 limit;

    if (D_800B6CE0 == D_800B6D18) {
        return;
    }
    D_800B6CE0 = D_800B6D18;
    if (func_00283BC0() == 0) {
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
    }
    func_00283BC0();
    if (D_800C1BB0->unknown60 != 0 && (D_800C1BB0->fade -= 2.0f * (D_800B6D28 * 0.06666667f)) < -0.5f) {
        D_800C1BB0->unknown60 = 0;
        D_800C1BB0->fade = 0.0f;
        if (func_00284174() != 0) {
            func_00284468(1);
            func_002844F0();
            if (D_800C1BB0->owner != 0) {
                func_002052D8(0, D_800C1BB0->owner);
            }
            D_800C1BB0->owner = 0;
            D_800C1BB0->resource = 0;
            D_800C1BB0->active = 0;
            D_800C1BB0->state3C = 0;
            D_800C1BB0->unknown60 = 0;
        }
        return;
    }
    func_00283BC0();
    if (func_00284174() != 0 && func_00284240() == 0 && D_801198A4.busy == 0 && D_801198A4.paused == 0) {
        if (D_800C1BB0->time >= D_800C1BB0->end && D_800C1BB0->initialized44 == 0) {
            CueCallback cue;

            func_00275904(-1);
            if (D_800C1BB0->unknown5C != 0) {
                func_00283A4C();
            }
            ctx = D_800C1BB0;
            cue = ctx->callback10;
            if (cue != 0) {
                ctx->callback10 = 0;
                cue();
            }
            D_800C1BB0->initialized44 = 1;
        }
        if (D_800C1BB0->time >= D_800C1BB0->clipEnd && D_800C1BB0->closed == 0) {
            if (D_800C1BB0->owner != 0) {
                func_002052D8(0, D_800C1BB0->owner);
            }
            {
                CueCallback end = D_800C1BB0->onClose;

                D_800C1BB0->owner = 0;
                D_800C1BB0->resource = 0;
                D_800C1BB0->active = 0;
                D_800C1BB0->state3C = 0;
                D_800C1BB0->unknown60 = 0;
                if (end != 0) {
                    D_800C1BB0->onClose = 0;
                    end();
                }
            }
            D_800C1BB0->closed = 1;
        }
    }
    func_00283BC0();
    if (D_800C1BB0->active != 0) {
        func_00206068(0xA);
    }
}
