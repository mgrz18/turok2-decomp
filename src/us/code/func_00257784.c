#include "common.h"
#include "structs/player.h"
#include "globals/player.h"
#include "globals/scene.h"
#include "protos/player.h"

s32 CPlayer__LoseLife(s32);

void PlayerDeath_CinemaDone(void) {
    if (sDeathCinemaPlayer->m_Data.m_Lives == 1) {
        CPlayer__LoseLife((sptr) sDeathCinemaPlayer);
    } else {
        CPlayer__SetState(sDeathCinemaPlayer, sDeathCinemaPlayer, 0x1F);
    }
    gCinemaHidesActors = 1;
    gDeathTint = 0;
}
