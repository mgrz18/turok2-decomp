#include "common.h"
#include "globals/player.h"

s32 Cinema_SetNextCallbacks(s32, s32, s32 *); /* extern */
extern s32 PlayerCinema50_Done;

void PlayerCinema50_Enter(s32 arg0) {
    sCinema50Player = arg0;
    Cinema_SetNextCallbacks(0x50, 0, &PlayerCinema50_Done);
}
