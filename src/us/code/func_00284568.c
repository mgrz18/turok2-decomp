#include "common.h"
#include "structs/cinema.h"
#include "globals/game.h"
#include "globals/cinema.h"

extern f32 gFrameIncrement;          /* frame time scale */

s32 Cinema_IsActive(void);
s32 Cinema_IsFinished(void);
void Cinema_Start(void);

/* Advances the clock at gCinemaPlayerP by one frame. */
void Cinema_AdvanceClock(void) {
    if (Cinema_IsActive()) {
        if (gEngineApp_m_Settings.paused != 0 || Cinema_IsFinished()) {
            gCinemaPlayerP->prevTime = gCinemaPlayerP->time;
            return;
        }
        gCinemaPlayerP->prevTime = gCinemaPlayerP->time;
        gCinemaPlayerP->time += gFrameIncrement * (1.0f / 15);
        if (gCinemaPlayerP->time >= gCinemaPlayerP->startTime) {
            Cinema_Start();
        }
        if (gCinemaPlayerP->time >= gCinemaPlayerP->clipEnd) {
            gCinemaPlayerP->time = gCinemaPlayerP->clipEnd;
            if (gCinemaPlayerP->stopAtEnd) {
                gCinemaPlayerP->finished = 1;
                gCinemaPlayerP->stopAtEnd = 0;
            }
        }
    }
}
