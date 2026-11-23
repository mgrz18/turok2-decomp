#include "common.h"
#include "structs/player.h"
#include "globals/player.h"

s32 Cinema_SetNextCallbacks(s32, sptr, s32 *); /* extern */
void PlayerCinema50_Done(void);

void PlayerCinema50_Enter(CPlayer *player) {
    sCinema50Player = player;
    Cinema_SetNextCallbacks(0x50, 0, &PlayerCinema50_Done);
}
