/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80243A80_6.CPP, func_802450CC_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/cinema.h"
#include "globals/game.h"

typedef struct GameState {
    s32 busy;               /* 0x0000 */
    u8 pad4[0x1450 - 0x4];
    s32 paused;             /* 0x1450 */
} GameState;

extern Cinema *gCinemaPlayerP;
extern s32 sCinemaLastFrame;      /* last frame handled */
extern s32 gFrameCount;      /* frame counter */
extern f32 gFrameIncrement;      /* frame time */
extern GameState gEngineApp_m_Game_m_PlayerList_m_bCinema;

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

    if (sCinemaLastFrame == gFrameCount) {
        return;
    }
    sCinemaLastFrame = gFrameCount;
    if (Cinema_StartPending() == 0) {
        if (Cinema_IsActive() != 0) {
            if (gEngineApp_m_Settings.paused != 0 || Cinema_IsFinished() != 0) {
                gCinemaPlayerP->prevTime = gCinemaPlayerP->time;
            } else {
                ctx = gCinemaPlayerP;
                ctx->prevTime = *(volatile f32 *)&ctx->time;
                ctx->time += gFrameIncrement * 0.06666667f;
                if (ctx->startTime <= ctx->time) {
                    Cinema_Start();
                }
                value = gCinemaPlayerP->time;
                limit = gCinemaPlayerP->clipEnd;
                if (limit <= value) {
                    gCinemaPlayerP->time = limit;
                    if (gCinemaPlayerP->stopAtEnd != 0) {
                        gCinemaPlayerP->finished = 1;
                        gCinemaPlayerP->stopAtEnd = 0;
                    }
                }
            }
        }
        Cinema_FireCues();
    }
    Cinema_StartPending();
    if (gCinemaPlayerP->fading != 0 && (gCinemaPlayerP->fade -= 2.0f * (gFrameIncrement * 0.06666667f)) < -0.5f) {
        gCinemaPlayerP->fading = 0;
        gCinemaPlayerP->fade = 0.0f;
        if (Cinema_IsActive() != 0) {
            Cinema_RunEnd(1);
            Cinema_Close();
            if (gCinemaPlayerP->owner != 0) {
                CCache__Deallocate(0, gCinemaPlayerP->owner);
            }
            gCinemaPlayerP->owner = 0;
            gCinemaPlayerP->resource = 0;
            gCinemaPlayerP->active = 0;
            gCinemaPlayerP->finished = 0;
            gCinemaPlayerP->fading = 0;
        }
        return;
    }
    Cinema_StartPending();
    if (Cinema_IsActive() != 0 && Cinema_IsFinished() == 0 && gEngineApp_m_Game_m_PlayerList_m_bCinema.busy == 0 && gEngineApp_m_Game_m_PlayerList_m_bCinema.paused == 0) {
        if (gCinemaPlayerP->time >= gCinemaPlayerP->endTime && gCinemaPlayerP->ended == 0) {
            CinemaCallback cue;

            Sound__StopAllExcept(-1);
            if (gCinemaPlayerP->restoreAtEnd != 0) {
                Cinema_RestorePlayer();
            }
            ctx = gCinemaPlayerP;
            cue = ctx->onEnd;
            if (cue != 0) {
                ctx->onEnd = 0;
                cue();
            }
            gCinemaPlayerP->ended = 1;
        }
        if (gCinemaPlayerP->time >= gCinemaPlayerP->clipEnd && gCinemaPlayerP->closed == 0) {
            if (gCinemaPlayerP->owner != 0) {
                CCache__Deallocate(0, gCinemaPlayerP->owner);
            }
            {
                CinemaCallback end = gCinemaPlayerP->onClose;

                gCinemaPlayerP->owner = 0;
                gCinemaPlayerP->resource = 0;
                gCinemaPlayerP->active = 0;
                gCinemaPlayerP->finished = 0;
                gCinemaPlayerP->fading = 0;
                if (end != 0) {
                    gCinemaPlayerP->onClose = 0;
                    end();
                }
            }
            gCinemaPlayerP->closed = 1;
        }
    }
    Cinema_StartPending();
    if (gCinemaPlayerP->active != 0) {
        CCache__SetFrameDelay(0xA);
    }
}
