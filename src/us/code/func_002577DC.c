#include "common.h"
#include "globals/player.h"

s32 CPlayer__LoseLife(s32);                         /* extern */

void PlayerCinema50_Done(void) {
    CPlayer__LoseLife(sCinema50Player);
}
