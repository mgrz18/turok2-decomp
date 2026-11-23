/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_4.CPP, func_80217B3C_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/player.h"
#include "globals/game.h"

#define ABS(a) (((a) < 0.0f) ? -(a) : (a))


/* Returns the 8-way stick direction (0 = up, clockwise) or -1 inside the dead zone. */
s32 CBarrel__GetStickDirection(sptr unused, CPlayer *player) {
    ControllerProfile *pad = player->m_ContState.m_pContState;
    s8 x = pad->stickX;
    s8 y = pad->stickY;

    if (x * x + y * y < 3600) {
        return -1;
    }
    if (ABS(ABS(x) - ABS(y)) < 46 && x != 0 && y != 0 && !gEngineApp_m_Settings.multiplayer) {
        if (x * y > 0) {
            if (x > 0) {
                return 1;
            }
            /* Redundant re-read kept from the original (Rage Wars has it too): it shapes the branch layout. */
            if (pad->stickX != 0) {
                return 5;
            }
            return 5;
        }
        return y > 0 ? 7 : 3;
    }
    if (ABS(x) > ABS(y)) {
        return x <= 0 ? 6 : 2;
    }
    if (y > 0) {
        return 0;
    }
    return 4;
}
