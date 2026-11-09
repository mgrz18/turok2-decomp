#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "protos/scene.h"

extern void CPlayerPool__SetInvincibility(void *players, f32 value);

void BlindOne_PhaseVictoryInit(void) {
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    CScene__UnmarkGeneratorsSaved(&gScene);
}
