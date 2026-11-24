#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "blindone_scene.h"
#include "protos/scene.h"

extern void CPlayerPool__SetInvincibility(void *players, f32 value);

void BlindOne_PhaseVictoryInit_2(void) {
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    CScene__UnmarkGeneratorsSaved(&gScene);
}
