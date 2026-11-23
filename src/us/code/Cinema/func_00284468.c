#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"

s32 Cinema_RestorePlayer(void);
void Sound__StopAllExcept(s32);

/* De-faked in round 21 (j4): the draft cleared the callback in two identical arms under a test of
 * gCinemaPlayerP; reading the cinema into a local first gives the ROM order (found by the permuter). */

/* Ends the playing cinema once: stops the sounds, gives the player back if asked, runs and clears
 * its end callback. */
void Cinema_RunEnd(s32 restore) {
    Cinema *cinema;
    CinemaCallback onEnd;

    if (gCinemaPlayerP->ended == 0) {
        Sound__StopAllExcept(-1);
        if (gCinemaPlayerP->restoreAtEnd != 0 || restore != 0) {
            Cinema_RestorePlayer();
        }
        cinema = gCinemaPlayerP;
        onEnd = cinema->onEnd;
        if (onEnd != NULL) {
            cinema->onEnd = NULL;
            onEnd();
        }
        gCinemaPlayerP->ended = 1;
    }
}
