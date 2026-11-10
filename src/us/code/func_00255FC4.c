#include "common.h"
#include "structs/player.h"
#include "globals/player.h"

s32 Cinema_SetNextCallbacks(s32, sptr, s32 *); /* extern */
extern s32 PlayerCinema50_Done;

void PlayerCinema50_Enter(CPlayer *player) {
    sCinema50Player = player;
    Cinema_SetNextCallbacks(0x50, 0, &PlayerCinema50_Done);
}
