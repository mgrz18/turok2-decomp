/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80243A80_6.CPP, func_802450CC_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/cinema.h"

typedef struct GameState {
    s32 busy;               /* 0x0000 */
    u8 pad4[0x1450 - 0x4];
    s32 paused;             /* 0x1450 */
} GameState;

extern Cinema *D_800C1BB0;
extern s32 D_800B6CE0;      /* last frame handled */
extern s32 D_800B6D18;      /* frame counter */
extern s32 D_8011ACA4;
extern f32 gFrameIncrement;      /* frame time */
extern GameState D_801198A4;

extern s32 Cinema_StartPending(void);
extern s32 Cinema_IsActive(void);
extern s32 Cinema_IsFinished(void);
extern void Cinema_Start(void);
extern void Cinema_RestorePlayer(void);
extern void Cinema_FireCues(void);
extern void Cinema_RunEnd(s32);
extern void Cinema_Close(void);
extern void CCache__Deallocate(void *, void *);
extern void Sound__StopAllExcept(s32);
extern void CCache__SetFrameDelay(s32);

void Cinema_Update(void) {
    Cinema *ctx;
    f32 value;
    f32 limit;

    if (D_800B6CE0 == D_800B6D18) {
        return;
    }
    D_800B6CE0 = D_800B6D18;
    if (Cinema_StartPending() == 0) {
        if (Cinema_IsActive() != 0) {
            if (D_8011ACA4 != 0 || Cinema_IsFinished() != 0) {
                D_800C1BB0->prevTime = D_800C1BB0->time;
            } else {
                ctx = D_800C1BB0;
                ctx->prevTime = *(volatile f32 *)&ctx->time;
                ctx->time += gFrameIncrement * 0.06666667f;
                if (ctx->startTime <= ctx->time) {
                    Cinema_Start();
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
        Cinema_FireCues();
    }
    Cinema_StartPending();
    if (D_800C1BB0->fading != 0 && (D_800C1BB0->fade -= 2.0f * (gFrameIncrement * 0.06666667f)) < -0.5f) {
        D_800C1BB0->fading = 0;
        D_800C1BB0->fade = 0.0f;
        if (Cinema_IsActive() != 0) {
            Cinema_RunEnd(1);
            Cinema_Close();
            if (D_800C1BB0->owner != 0) {
                CCache__Deallocate(0, D_800C1BB0->owner);
            }
            D_800C1BB0->owner = 0;
            D_800C1BB0->resource = 0;
            D_800C1BB0->active = 0;
            D_800C1BB0->finished = 0;
            D_800C1BB0->fading = 0;
        }
        return;
    }
    Cinema_StartPending();
    if (Cinema_IsActive() != 0 && Cinema_IsFinished() == 0 && D_801198A4.busy == 0 && D_801198A4.paused == 0) {
        if (D_800C1BB0->time >= D_800C1BB0->endTime && D_800C1BB0->ended == 0) {
            CinemaCallback cue;

            Sound__StopAllExcept(-1);
            if (D_800C1BB0->restoreAtEnd != 0) {
                Cinema_RestorePlayer();
            }
            ctx = D_800C1BB0;
            cue = ctx->onEnd;
            if (cue != 0) {
                ctx->onEnd = 0;
                cue();
            }
            D_800C1BB0->ended = 1;
        }
        if (D_800C1BB0->time >= D_800C1BB0->clipEnd && D_800C1BB0->closed == 0) {
            if (D_800C1BB0->owner != 0) {
                CCache__Deallocate(0, D_800C1BB0->owner);
            }
            {
                CinemaCallback end = D_800C1BB0->onClose;

                D_800C1BB0->owner = 0;
                D_800C1BB0->resource = 0;
                D_800C1BB0->active = 0;
                D_800C1BB0->finished = 0;
                D_800C1BB0->fading = 0;
                if (end != 0) {
                    D_800C1BB0->onClose = 0;
                    end();
                }
            }
            D_800C1BB0->closed = 1;
        }
    }
    Cinema_StartPending();
    if (D_800C1BB0->active != 0) {
        CCache__SetFrameDelay(0xA);
    }
}
