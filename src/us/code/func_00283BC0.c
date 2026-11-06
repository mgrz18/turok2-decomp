/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80243A80_6.CPP, func_80244FB4_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/cinema.h"
#include "globals/game.h"
#include "globals/cinema.h"

extern f32 gFrameIncrement;      /* frame time */

extern void Cinema_Load(s32);
extern s32 Cinema_IsActive(void);
extern s32 Cinema_IsFinished(void);
extern void Cinema_Start(void);
extern void Cinema_FireCues(void);

s32 Cinema_StartPending(void) {
    s32 none;
    Cinema *ctx;
    f32 value;
    f32 limit;

    none = -1;
    if (gCinemaPlayerP->entryIndex == none) {
        return 0;
    }
    Cinema_Load(gCinemaPlayerP->entryIndex);
    gCinemaPlayerP->entryIndex = none;
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
    return 1;
}
