#include "common.h"
#include "structs/player.h"
#include "globals/player.h"

s32 CPlayer__LoseLife(CPlayer *player);

void PlayerCinema50_Done(void) {
    CPlayer__LoseLife(sCinema50Player);
}
